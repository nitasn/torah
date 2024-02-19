importScripts('engine.js');

Module.onRuntimeInitialized = () => {
  self.c_style__search = Module.cwrap('c_style__search', null, ['string', 'number', 'number']);
  postMessage({ type: 'ready' });
};

onmessage = ({ data }) => {
  const { pattern, search_id } = data;

  // malloc 8 bytes for two int32 numbers
  const pIndex = Module._malloc(8);
  const pStep = pIndex + 4;

  c_style__search(pattern + '\0', pIndex, pStep);

  const [index, step] = new Int32Array(Module.HEAP32.buffer, pIndex, 2);
  Module._free(pIndex);

  postMessage({
    type: 'done',
    result: ~index ? { index, step } : null,
    search_id,
  });
};


// TODO: don't exit(3) actually thow exception (if that makes the wasm module persist)
// unless core persists anyway, in that case, whatever