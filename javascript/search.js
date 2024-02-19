(() => {

let engineLoaded = false;

const engine = new Worker('worker.js');

engine.onmessage = ({ data }) => {
  if (data.type === 'ready') {
    engineLoaded = true;
    console.log('Engine is Ready');
  }
  else if (data.type === 'done') {
    engineSays(data);
  }
};

engine.onerror = () => {
  // todo some ui msg
};

const callbacksRegistery = new Map();

function engineSays({ result, search_id }) {
  const resolvePromise = callbacksRegistery.get(search_id);
  callbacksRegistery.delete(search_id);
  resolvePromise(result);
}

let _next_id = 0;

/**
 * Equidistant Letter Sequence ("דילוגי אותיות") Search in the Torah, in Hebrew.
 * @param {string} pattern in Hebrew
 * @returns {Promise<{index: number, step: number} | null>} `{ index, number }` or `null`
 */
function search(pattern) {
  if (!engineLoaded) throw new Error("engine is not ready");
  let resolvePromise;
  const promise = new Promise((resolve) => (resolvePromise = resolve));
  callbacksRegistery.set(_next_id, resolvePromise);
  engine.postMessage({ pattern, search_id: _next_id++ });
  return promise;
};

// module export
globalThis.search = search;

globalThis.hasEngineLoaded = () => engineLoaded;

})();
