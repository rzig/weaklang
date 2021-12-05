import React, { useEffect, useRef, useState } from 'react';
import './App.css';

import Editor from "@monaco-editor/react";

function App() {
  const editor = useRef<any>();

  const [output, setOutput] = useState("");

  useEffect(() => {
  }, []);

  const run = () => {
    console.log("editor.current is " + editor.current);
    const program = editor.current.getValue();
    // @ts-ignore
    let ptr = allocate(intArrayFromString(program), ALLOC_NORMAL);
    // @ts-ignore
    let ret = Module._execute_program(ptr);
    // @ts-ignore
    let res = UTF8ToString(ret);
    console.log("Program output: ");
    console.log(res);
    setOutput(res);
  };

  return (
    <>
      <nav style={{display: "flex", flexDirection: "row", alignContent: "center", alignItems: "center", justifyContent: "center", height: "10vh", backgroundColor: "#ccc"}}>
        <div>
          <button style={{display: "block"}} onClick={run}>Run Code</button>
        </div>
      </nav>
      <div style={{display: "flex", flexDirection: "row"}}>
        <Editor
          height="90vh"
          width="50vw"
          defaultLanguage="plaintext"
          defaultValue="//Weak"
          onMount={(editor_b, monaco) => {
            editor.current = editor_b;
            console.log("editor.current is " + editor.current)
          }}
        />
        <Editor
          height="90vh"
          width="50vw"
          defaultLanguage="plaintext"
          value={output}
        />
      </div>
    </>
  );
}

export default App;
