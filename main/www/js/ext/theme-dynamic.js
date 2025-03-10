define("ace/theme/dynamic", ["require", "exports", "module", "ace/lib/dom"], function(require, exports, module) {
    "use strict";
    
    exports.isDark = false;
    exports.cssText = ".ace-dynamic .ace_gutter {\
    background: var(--background-active); \
    color: rgb(136, 136, 136); \
    }\
    .ace-dynamic .ace_print-margin {\
    width: 1px; \
    background: var(--background-active); \
    }\
    .ace-dynamic {\
    background-color: var(--background); \
    color: var(--text-color); \
    }\
    .ace-dynamic .ace_fold {\
    background-color: var(--token-function, #22863a); \
    }\
    .ace-dynamic .ace_cursor {\
    color: var(--text-color); \
    }\
    .ace-dynamic .ace_storage, \
    .ace-dynamic .ace_keyword, \
    .ace-dynamic .ace_variable {\
    color: var(--token-keyword, #d73a49); \
    }\
    .ace-dynamic .ace_constant.ace_buildin {\
    color: var(--token-builtin, #6f42c1); \
    }\
    .ace-dynamic .ace_constant.ace_library {\
    color: var(--token-library, #005cc5); \
    }\
    .ace-dynamic .ace_function {\
    color: var(--token-function, #22863a); \
    }\
    .ace-dynamic .ace_string {\
    color: var(--token-string,rgb(0, 97, 207)); \
    }\
    .ace-dynamic .ace_comment {\
    color: var(--token-comment, #6a737d); \
    }\
    .ace-dynamic .ace_comment.ace_doc {\
    color: var(--token-comment, #6a737d); \
    }\
    .ace-dynamic .ace_comment.ace_doc.ace_tag {\
    color: var(--token-comment, #6a737d); \
    }\
    .ace-dynamic .ace_constant.ace_numeric {\
    color: var(--token-numeric, #e36209); \
    }\
    .ace-dynamic .ace_tag {\
    color: var(--token-tag, #22863a); \
    }\
    .ace-dynamic .ace_type {\
    color: var(--token-type, #6f42c1); \
    }\
    .ace-dynamic .ace_xml-pe {\
    color: var(--token-xml, #d73a49); \
    }\
    .ace-dynamic .ace_marker-layer .ace_selection {\
    background: var(--selection-bg, rgba(181, 213, 255, 0.5)); \
    }\
    .ace-dynamic .ace_marker-layer .ace_bracket {\
    margin: -1px 0 0 -1px; \
    border: 1px solid rgb(192, 192, 192); \
    }\
    .ace-dynamic .ace_meta.ace_tag {\
    color: var(--token-tag, #22863a); \
    }\
    .ace-dynamic .ace_invisible {\
    color: #ddd; \
    }\
    .ace-dynamic .ace_entity.ace_other.ace_attribute-name {\
    color: var(--token-keyword, #d73a49); \
    }\
    .ace-dynamic .ace_marker-layer .ace_step {\
    background: rgb(255, 255, 0); \
    }\
    .ace-dynamic .ace_active-line {\
    background: var(--background-active, rgba(232, 242, 254, 0.3)); \
    }\
    .ace-dynamic .ace_gutter-active-line {\
    background-color: var(--background-active); \
    }\
    .ace-dynamic .ace_marker-layer .ace_selected-word {\
    border: 1px solid rgb(181, 213, 255); \
    }\
    .ace-dynamic .ace_indent-guide {\
    background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAACCAYAAACZgbYnAAAAE0lEQVQImWP4////f4bLly//BwAmVgd1/w11/gAAAABJRU5ErkJggg==\") right repeat-y; \
    }";
    
    exports.cssClass = "ace-dynamic";
    
    var dom = require("../lib/dom");
    dom.importCssString(exports.cssText, exports.cssClass, false);
});
(function() {
    window.require(["ace/theme/dynamic"], function(m) {
        if (typeof module === "object" && typeof exports === "object" && module) {
            module.exports = m;
        }
    });
})();
