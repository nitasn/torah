importScripts('search.js');

let searchPackedResult;

Module.onRuntimeInitialized = async () => {
  postMessage({ type: 'ready' });
  searchPackedResult = Module.cwrap('search__packed_result', 'number', ['string']);
};

onmessage = ({ data }) => {
  const { pattern } = data;
  const bits = searchPackedResult(pattern + '\0');
  postMessage({ type: 'result', result: unpackResult(bits) });
};

function unpackResult(bits) {
  if (bits === 0) return null;

  const twenty_bits = (1 << 20) - 1;

  return {
    index: (bits >> 1) & twenty_bits,
    step: bits >> 21,
  };
}


// TODO: don't exit(3) actually thow exception (if that makes the wasm module persist)
// unless core persists anyway, in that case, whatever