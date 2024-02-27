/**
 * We have `NUM_WORKERS` threads running the engine.
 * 
 * Upon query, we split the work between them.
 * We resolve the search as soon as any of them finds a solution,
 * Or after all of them returned empty-handed.
 * 
 * The `search` function we export takes a pattern and an optional { min_step, max_step } object;
 * It returns a promise that eventually resolves to either an { index, step } object, or to null.
 */


// half the CPU cores, or 2 if cores info is unavailable
const NUM_WORKERS = Math.round(navigator.hardwareConcurrency * 0.5) || 2;


(() => { // module iife

///////////////////////////////////////////////////////////////
///            M A N A G I N G   S E A R C H E S            ///
///////////////////////////////////////////////////////////////

const newID = (() => {
  let id = 0;
  return () => id++;
})();

/**
 * divide a search into ranges of steps to check (ranges.length <= NUM_WORKERS)
 * @returns {{ min_step: number, max_step: number }[]}
 */
function splitRangeOfStepsBetweenWorkers(min_step, max_step, pattern_len) {
  const torah_block_len = 304805;
  const max_possible_step = Math.floor((torah_block_len - 1) / (pattern_len - 1));

  if (!max_step || max_step > max_possible_step) max_step = max_possible_step;

  if (!min_step) min_step = 1;

  if (min_step > max_step) {
    // there are no steps to check
    return [];
  }

  if (max_step - min_step < 1000) {
    // a single thread can do it quickly
    return [{ min_step, max_step }];
  }

  const ranges = [];

  const chunk_size = (max_step + 1 - min_step) / NUM_WORKERS;
  let _current_min = min_step;

  for (let i = 1; i <= NUM_WORKERS; ++i) {
    let _min_step = Math.round(_current_min);
    let _max_step = Math.round(_current_min += chunk_size) - 1;

    if (i == NUM_WORKERS) _max_step = max_step;

    ranges.push({ min_step: _min_step, max_step: _max_step });
  }

  return ranges;
}

/**
 * key: search_id
 * value: { numWorkersInvolved, numWorkersDone, resolveSearch }
 */
const searchesRegistery = new Map();

/**
 * Equidistant Letter Sequence ("דילוגי אותיות") Search in the Torah, in Hebrew.
 * Also known as "The Bible Code"
 * @param {string} pattern in Hebrew
 * @param {{ min_step: number?, max_step: number? }}
 * @returns {Promise<{index: number, step: number} | null>} `{ index, number }` or `null`
 */
function search(pattern, { min_step = null, max_step = null } = {}) {
  if (!isEngineReady()) {
    throw new Error(`engine isn't ready yet (num workers loaded: ${numWorkersLoaded} / ${NUM_WORKERS})`);
  }

  // split the range [min_step, max_step] into up to NUM_WORKERS sub ranges
  const subRanges = splitRangeOfStepsBetweenWorkers(+min_step, +max_step, pattern.length);

  if (subRanges.length === 0) {
    console.warn("no steps to check");
    return Promise.resolve(null);
  }

  const search_id = newID();

  const searchPromise = new Promise((resolveSearch) => {
    searchesRegistery.set(search_id, {
      numWorkersInvolved: subRanges.length,
      numWorkersDone: 0,
      resolveSearch,
    });
  });

  for (let i = 0; i < subRanges.length; ++i) {
    const { min_step, max_step } = subRanges[i];
    workers[i].postMessage({ search_id, pattern, min_step, max_step });
  }

  return searchPromise;
};

function onEngineSearchDone({ result, search_id }) {
  const searchInstance = searchesRegistery.get(search_id);
  if (result) {
    // if already resolved, this is a noop
    searchInstance.resolveSearch(result);
  }
  if (++searchInstance.numWorkersDone == searchInstance.numWorkersInvolved) {
    searchesRegistery.delete(search_id);
    // if already resolved, this is a noop
    searchInstance.resolveSearch(null);
  }
}

///////////////////////////////////////////////////////////////
///      M A N A G I N G   W O R K E R   T H R E A D S      ///
///////////////////////////////////////////////////////////////

const newWorker = () => new Worker('worker.js');
const workers = Array.from({ length: NUM_WORKERS }, newWorker);
let numWorkersLoaded = 0;

workers.forEach((worker) => {
  worker.onmessage = ({ data }) => {
    if (data.type === 'ready' && ++numWorkersLoaded == NUM_WORKERS) {
      console.log('Engine is Ready');
    }
    if (data.type === 'done') {
      onEngineSearchDone(data);
    }
  };
  // worker.onerror = { /* todo some ui indication? */ };
});

///////////////////////////////////////////////////////////////
///               M O D U L E   E X P O R T S               ///
///////////////////////////////////////////////////////////////

globalThis.search = search;

globalThis.isEngineReady = () => (numWorkersLoaded == NUM_WORKERS);

})(); // module iife
