mergeInto(LibraryManager.library, {
  wasm_console_setup_event: function() {
    document.getElementById("output").addEventListener("keydown", function(event) {
      let ch = event.which || event.keyCode;
      event.preventDefault();
      if (typeof window._console_buffer !== "string")
        window._console_buffer = "";
      if (event.isComposing || (event.keyCode === 229))
        return;
      if (ch === 38)
        window._console_buffer += "\x1b\x5b\x41";
      else if (ch === 40)
        window._console_buffer += "\x1b\x5b\x42";
      else if (ch === 39)
        window._console_buffer += "\x1b\x5b\x43";
      else if (ch === 37)
        window._console_buffer += "\x1b\x5b\x44";
      else if ((ch === 8) || (ch === 9) || (ch === 13))
        window._console_buffer += String.fromCharCode(ch);
      else if ((ch === 16) || (ch === 17) || (ch === 18) || (ch === 20) || (ch === 27))
        window._console_buffer += "";
      else if (ch === 46)
        window._console_buffer += "\x7f";
      else
        window._console_buffer += event.key;
      Module.ccall('wasm_user_input', null, ["string", "number"], [window._console_buffer, window._console_buffer.length]);
      window._console_buffer = "";
    });
    // document.getElementById("output").addEventListener("input", function(event) {
    //   event.preventDefault();
    //   console.log(event.data);
    // });
  }
});
 