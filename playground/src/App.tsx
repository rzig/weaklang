import React, { useEffect } from 'react';
import logo from './logo.svg';
import './App.css';


function App() {

  useEffect(() => {
    // @ts-ignore
    let ptr = allocate(intArrayFromString("p 1 + 1; p \"Hi\";"), ALLOC_NORMAL);
    // @ts-ignore
    let ret = Module._execute_program(ptr);
    // @ts-ignore
    let res = UTF8ToString(ret);
    console.log("Program output: ");
    console.log(res);
  })

  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.tsx</code> and save to reload.
        </p>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn React
        </a>
      </header>
    </div>
  );
}

export default App;
