import React, { useRef, useState } from 'react';
import './App.css';

import Editor from "@monaco-editor/react";

function App() {
  const editor = useRef<any>();

  const [output, setOutput] = useState("");

  const run = () => {
    const program = editor.current.getValue();
    // @ts-ignore
    let ptr = allocate(intArrayFromString(program), ALLOC_NORMAL);
    let ret;
    try {
      // @ts-ignore
      ret = Module._execute_program(ptr); 
    } catch (error) {
      // @ts-ignore
      console.log(Module.ExceptionInfo(error));
    }
    // @ts-ignore
    let res = UTF8ToString(ret);
    // @ts-ignore
    _free(ptr);
    setOutput(res);
  };

  return (
    <div style={{height: "100vh", display: "flex", flexDirection: "column", maxHeight: "100vh"}}>
      <nav style={{display: "flex", flexDirection: "row", alignContent: "center", alignItems: "center", justifyContent: "space-between", height: "10vh", backgroundColor: "#EDEDED", paddingLeft: "30px", paddingRight: "30px"}}>
        <div style={{width: 300, display: "flex", flexGrow: 1, alignItems: "center"}}>
          <h1 style={{fontFamily: "'Inter', sans-serif", fontSize: "1.6rem"}}>WeakLang Playground</h1>
        </div>
        <div style={{display: "flex"}}>
          <a style={{display: "block", backgroundColor: "#81d4fa ", borderColor: "#03a9f4", borderStyle: "solid", borderWidth: "2px", borderRadius: "5px", padding: "16px", cursor: "pointer", fontFamily: "'Inter', sans-serif", fontSize: "1.15rem", marginRight: "15px", textDecoration: "none", color: "black"}} onClick={run} href="https://github.com/rzig/weaklang">See Github</a>
          <button style={{display: "block", backgroundColor: "#2BFFA7", borderColor: "#21BF7D", borderStyle: "solid", borderWidth: "2px", borderRadius: "5px", padding: "16px", cursor: "pointer", fontFamily: "'Inter', sans-serif", fontSize: "1.15rem"}} onClick={run}>Run Code</button>
        </div>
      </nav>
      <div style={{flexGrow: 1, height: "100%"}}>
        <div style={{width: "100%", height: "100%"}}>
          <div style={{display: "flex", flexDirection: "row", padding: "30px", height: "90%"}}>
            <div style={{borderRadius: 7, border: "1px solid #DEDEDE", height: "100%", flexGrow: 1, marginRight: "30px", width: "50%", overflow: "hidden"}}>
              <div style={{width: "100%", height: "100%"}}>
                <Editor
                  defaultLanguage="plaintext"
                  defaultValue="p T;"
                  onMount={(editor_b, monaco_b) => {
                    editor.current = editor_b;
                  }}
                  options={{minimap: {enabled: false}, automaticLayout: true, fontFamily: "Roboto Mono", fontSize: 16}}
                />
              </div>
            </div>

            <div style={{borderRadius: 7, border: "1px solid #DEDEDE", height: "100%", flexGrow: 1, width: "50%", overflow: "hidden"}}>
              <div style={{width: "100%", height: "100%"}}>
                <Editor
                  defaultLanguage="plaintext"
                  value={output}
                  options={{minimap: {enabled: false}, readOnly: true, fontFamily: "Roboto Mono", fontSize: 16}}
                />
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}

export default App;
