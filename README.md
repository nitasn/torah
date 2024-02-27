# Fast Client-Side "Bible Code" Search in Hebrew
Fast Equidistant-Letter-Sequence ("דילוגי אותיות") Search in the Torah. There's no server-side.

## Engine
* Written in C++ (runs as a WASM on client-side).
* Compiled into WebAssembly using emscripten.
* Boyer-Moore's algorithm adjusted for E.L.S.
* Multi-Threaded (using **JS Workers**, as [std::thread has serious limitations](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/SharedArrayBuffer#security_requirements)).

## UI
 * Using [pico.css](picocss.com) for fast bootstrapping.
