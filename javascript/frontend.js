const main = document.querySelector("main");
const form = document.getElementById("search-form");
const searchBtn = document.getElementById("search-btn");
const searchInput = document.getElementById("search-input");
const minStepInput = document.getElementById("min-step-input");
const maxStepInput = document.getElementById("max-step-input");
const resultTemplate = document.getElementById('result-template');
const searchIndicator = document.getElementById("search-indicator");


function inputLength() {
  return searchInput.value.match(/[א-ת]/g)?.length || 0;
}

function onInput() {
  searchInput.value = searchInput.value.trimStart();
  searchBtn.disabled = !inputLength();

  if (maxStepInput.value !== '') {
    maxStepInput.value = Math.min(maxPossibleStep(), +maxStepInput.value);

    if (minStepInput.value !== '') {
      minStepInput.value = Math.min(+minStepInput.value, +maxStepInput.value);
    }
  }
  else if (minStepInput.value !== '') {
    minStepInput.value = Math.min(maxPossibleStep(), +minStepInput.value);
  }
}
searchInput.addEventListener('input', onInput);
onInput();


let workingOnIt = false;
form.addEventListener('submit', async (event) => {
  event.preventDefault();
  if (workingOnIt || inputLength() === 0) return;

  if (!globalThis.isEngineReady?.()) {
    return openModal();
  }

  searchBtn.style.display = 'none';
  searchIndicator.style.display = 'unset';
  workingOnIt = true;

  // remove non-hebrew letters, and normalize spaces
  searchInput.value = searchInput.value.replace(/[^א-ת\s]/g, '').replace(/\s+/g, ' ').trim();

  const query = [searchInput.value, minStepInput.value, maxStepInput.value];

  const result = await search(searchInput.value, {
    min_step: +minStepInput.value,
    max_step: +maxStepInput.value,
  });

  searchBtn.style.display = 'unset';
  searchIndicator.style.display = 'none';
  workingOnIt = false;

  addResult(query, result);
});

function maxPossibleStep() {
  const torahLength = 304805;
  const patternLength = inputLength();
  if (patternLength === 0) return Number.POSITIVE_INFINITY;
  return Math.floor((torahLength - 1) / (patternLength - 1));
}

function addResult([pattern, minStep, maxStep], result) {
  const resultElement = resultTemplate.content.cloneNode(true);
  const titleElement = resultElement.querySelector('.title');
  const infoElement = resultElement.querySelector('.info');
  const textElement = resultElement.querySelector('.text');
  
  titleElement.innerText = `${pattern}`

  if (minStep || maxStep) {
    if (minStep) infoElement.innerText = `מדילוג ${minStep} `;
    if (maxStep) infoElement.innerText += `עד דילוג ${maxStep}`;
  } else {
    infoElement.innerText = 'דילוג לא מוגבל';
  }

  if (result) {
    textElement.innerText = `נמצא בדילוג ${result.step} במקום ${result.index}`;
    textElement.classList.add("green");
  }
  else {
    textElement.innerText = "לא נמצא";
    textElement.classList.add("red");
  }

  main.insertBefore(resultElement, document.querySelector('.result'));
}