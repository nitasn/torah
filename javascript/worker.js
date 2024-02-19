importScripts('search.js');
importScripts('iso-8859-8.js');

let searchPackedResult;

Module.onRuntimeInitialized = async () => {
  postMessage({ type: 'ready' });
  searchPackedResult = Module.cwrap('search__packed_result', 'number', ['string']);
};

onmessage = ({ data }) => {
  const { pattern } = data;
  const result = searchPackedResult(iso88598.encode(pattern) + '\0');
  postMessage({ type: 'result', result });
};


// worker.postMessage({ pattern: 'אברהם' });

// isoHebrew_to_number failed: non iso-8859-8 hebrew letter

// TODO: find it! throw in c++; in catch print whole string char by char!

// TODO: don't exit(3) actually thow exception (if that makes the wasm module persist)
// unless core persists anyway, in that case, whatever