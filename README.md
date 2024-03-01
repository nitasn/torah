# Fast In-Browser "Bible Code" Search in Hebrew
Fast Equidistant-Letter-Sequence ("דילוגי אותיות") Search in the Torah.

[Try it here!](https://nitasn.github.io/bible-code-searcher/)

## Engine
* Written in C++ (runs as a WASM on client-side).
* Compiled into WebAssembly using emscripten.
* Boyer-Moore's algorithm adjusted for E.L.S.
* Multi-Threaded (using **JS Workers**, as [std::thread has serious limitations](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/SharedArrayBuffer#security_requirements)).

## Front End
 * Using [pico.css](picocss.com) for fast bootstrapping.

## Run Locally
*Reminder: You can try it online. There's a link above.*

1. Make sure [emscripten](https://emscripten.org/) is installed.
2. Build the wasm:
```bash
git clone https://github.com/nitasn/torah
cd torah
make wasm
```
3. Serve the `frontend` directory over HTTP:
```bash
python3 -m http.server --directory frontend
```
4. Open your browser at [http://localhost:8000/](http://localhost:8000/).
