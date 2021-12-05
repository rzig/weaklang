import React, { useRef, useState } from 'react';
import './App.css';
import Editor from "@monaco-editor/react";

const defaultProgram = `# Weak is a programming language created by Russel Arbore, Tony Helsel,
# Chris Sapinski, and Ryan Ziegler, for our CS128 final project. It's 
# an interpreted language designed to make you more productive, with 
# simple keywords like "f", "p", or "w" instead of cumbersome, time-consuming
# ones like "for", "print", or "while". And, when using Weak on your computer
# (not browser), we provide high-performance matrix computations with the
# BLAS library. We hope you enjoy writing Weak as much as we did creating it.

# We've included a sample program below, and you can find complete language
# documentation on our GitHub (https://github.com/rzig/weaklang).

# Program prompt: https://adventofcode.com/2021/day/1

f dim(mat) {
    r (s (s mat))[0];
}

f len(list) {
    v dim(list) == 1;
    r (s list)[0];
}

f part_one(depths) {
    v dim(depths) == 1;
    a len = len(depths);
    v len >= 1;
    a j = 1;
    a count = 0;
    w (j < len) {
        i (depths[j] > depths[j-1]) {
            count = count + 1;
        }
        j = j + 1;
    }
    r count;
}

f part_two(depths) {
    v dim(depths) == 1;
    a len = len(depths);
    v len >= 1;
    a j = 0;
    a new_depths = [0] sa [len];
    a count = 0;
    w (j < len - 2) {
        new_depths[count] = depths[j] + depths[j+1] + depths[j+2];
        count = count + 1;
        j = j + 1;
    }
    r part_one(new_depths);
}

a d = [199, 200, 208, 210, 200, 207, 240, 269, 260, 263];

p part_one(d); # Should print 7
p part_two(d); # Should print 5
`

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
                  defaultValue={defaultProgram}
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
