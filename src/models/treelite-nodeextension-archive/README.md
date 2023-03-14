# So you want to run a Treelite tree in Node

We're going to use Treelite's C export to make a custom Node add-on.


* src'ed `node_api.h`
* discovered with find that `node_api.h` was in `/usr/include/node/node_api.h` and gcc was looking
in `/usr/include` already. Changed `node_api.h` to `node/node_api.h`, which worked
* used addon.c from [https://github.com/nodejs/node-addon-examples/tree/main/2_function_arguments/napi]
* updated bindings.gyp to add mymodel.c
* added package.json from github above 
* added bindings
* added addon.js
* ran it with node ./ - didn't work
* installed `nopt`
* ran `node gyp`
* sudo npm install -g node-pre-gyp
* ran `npx node-gyp`, worked
