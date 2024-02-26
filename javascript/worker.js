importScripts('engine.js');

Module.onRuntimeInitialized = () => {
  self.c_style__search = Module.cwrap('c_style__search', 'number', ['string', 'number', 'number', 'number', 'number']);
  postMessage({ type: 'ready' });
};

onmessage = ({ data }) => {
  const { search_id, pattern, min_step, max_step } = data;

  // malloc 8 bytes for two uint32 numbers
  const pIndex = Module._malloc(8);
  const pStep = pIndex + 4;

  const did_find = c_style__search(pattern + '\0', +min_step || 0, +max_step || 0, pIndex, pStep);

  const [index, step] = new Int32Array(Module.HEAP32.buffer, pIndex, 2);
  Module._free(pIndex);

  postMessage({
    type: 'done',
    search_id,
    result: did_find ? { index, step } : null,
  });
};