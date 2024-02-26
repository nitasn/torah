const NUM_WORKERS = 2;

// /* module start */ (() => {

let numWorkersLoaded = 0;

const newWorker = () => new Worker('worker.js');
const workers = Array.from({ length: NUM_WORKERS }, newWorker);

workers.forEach((worker) => {
  worker.onmessage = ({ data }) => {
    if (data.type === 'ready') {
      if (++numWorkersLoaded == NUM_WORKERS) {
        console.log('Engine is Ready');
      }
    }
    else if (data.type === 'done') {
      onEngineSearchDone(data);
    }
  };
  worker.onerror = () => {
    // todo some ui msg
  };
});

/**
 * Get ranges of steps to search (ranges.length <= NUM_WORKERS)
 * @returns {{ min_step: number, max_step: number }[]}
 */
function splitRangeOfStepsBetweenWorkers(min_step, max_step, pattern_len) {
  const torah_block_len = 304805;
  const max_possible_step = Math.floor((torah_block_len - 1) / (pattern_len - 1));

  if (!max_step || max_step > max_possible_step) max_step = max_possible_step;

  if (!min_step) min_step = 1;

  if (min_step > max_step) {
    // there's no valid step to check
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

const searchesRegistery = new Map();

function onEngineSearchDone({ result, search_id }) {
  const searchRegistery = searchesRegistery.get(search_id);
  searchesRegistery.result ??= result;
  if (++searchRegistery.numWorkersDone == searchRegistery.numWorkersInvolved) {
    searchesRegistery.delete(search_id);
    searchRegistery.resolveSearch(searchesRegistery.result);
  }
}

let search_id = 0;

/**
 * Equidistant Letter Sequence ("דילוגי אותיות") Search in the Torah, in Hebrew.
 * Also known as "The Bible Code"
 * @param {string} pattern in Hebrew
 * @param {{ min_step: number?, max_step: number? }}
 * @returns {Promise<{index: number, step: number} | null>} `{ index, number }` or `null`
 */
function search(pattern, { min_step = null, max_step = null } = {}) {
  if (!isEngineReady()) {
    throw new Error(
      `engine isn't ready yet (num workers loaded: ${numWorkersLoaded} / ${NUM_WORKERS})`
    );
  }

  const rangesOfSteps = splitRangeOfStepsBetweenWorkers(+min_step, +max_step, pattern.length);

  if (rangesOfSteps.length == 0) {
    /** TODO */
    return console.warn("no steps to check");
  }

  const searchPromise = new Promise((resolveSearch) => {
    searchesRegistery.set(search_id, {
      numWorkersInvolved: rangesOfSteps.length,
      numWorkersDone: 0,
      result: null,
      resolveSearch,
    });
  });

  for (let i = 0; i < rangesOfSteps.length; ++i) {
    const { min_step, max_step } = rangesOfSteps[i];
    workers[i].postMessage({ search_id, pattern, min_step, max_step });
  }

  search_id++; // for next search
  return searchPromise;
};

// module export
globalThis.search = search;

globalThis.isEngineReady = () => (numWorkersLoaded == NUM_WORKERS);

// /* module end */ })();
