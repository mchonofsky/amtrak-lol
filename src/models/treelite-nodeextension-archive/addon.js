var addon = require('bindings')('addon.node')
console.log("should be 0.0:", addon.predict(0.0,null,null,37.66911604446237,-120.9122489722034,86.23839701698081,37.66912176647264,-120.91223419501031,358.0));
console.log("should be 0.0:", addon.predict(0.0,null,null,38.90475479709952,-77.0035804709418,8.909838488234211,38.9050222243533,-77.00340443021516,514.0));
console.log("should be 0.0:", addon.predict(0.0,null,null,41.29822999589455,-72.92530173413981,18.24314251508335,41.29856457533378,-72.92497858945246,518.0));
console.log("should be 0.0:", addon.predict(0.0,null,null,40.73805299498814,-74.15814358874506,36.075202992703275,40.73780442903738,-74.15851683270769,150.0));
console.log("should be 0.0:", addon.predict(0.0,null,null,38.90356079684168,-77.00403823501145,20.103270402295145,38.90390654685579,-77.00387641733855,149.0));
console.log("should be 1.0:", addon.predict(0.0,null,null,40.7559095989678,-74.08770899728886,27.704407553844064,40.755780772010446,-74.08815773446129,415.0));
console.log("should be 0.0:", addon.predict(0.0,null,null,41.87805267718013,-87.6375106520768,0.8633551442876584,41.87808596495534,-87.63748018847507,14.0));
console.log("should be 0.0:", addon.predict(0.0,null,null,38.90475479709952,-77.0035804709418,8.867768344366484,38.90504396562596,-77.00340069713694,151.0));
console.log("should be 1.0:", addon.predict(0.0,0.0,1.0,47.58838452171854,-122.33151208121876,1.69634294509888,47.59878013075935,-122.32959655102304,432.0));
console.log("should be 1.0:", addon.predict(0.0,0.0,1.0,47.58838452171854,-122.33151208121876,31.3668098449707,47.61032367753732,-122.34356732325207,479.0));
console.log("should be 1.0:", addon.predict(43.2163505554199,0.0,1.0,29.694145091405435,-81.66036547503094,76.5591201782227,29.75325871632228,-81.64078431424208,272.0));
console.log("should be 1.0:", addon.predict(43.2163505554199,0.0,1.0,29.694145091405435,-81.66036547503094,79.6100463867188,29.845973958257947,-81.62041593961251,575.0));
console.log("should be 1.0:", addon.predict(60.1177635192871,0.0,1.0,43.0731286202348,-78.92203840429943,0.0,43.1225619232415,-78.9353729730449,361.0));
console.log("should be 1.0:", addon.predict(60.1177635192871,0.0,1.0,43.0731286202348,-78.92203840429943,14.9136791229248,43.12606928909525,-78.93926730178029,541.0));
console.log("should be 1.0:", addon.predict(58.3280982971191,0.0,1.0,42.20155742562044,-87.86398900545218,77.9944763183594,42.28196583921275,-87.89740037854888,291.0));
console.log("should be 1.0:", addon.predict(0.0,0.0,1.0,41.29910840804458,-72.92590113638599,17.9067039489746,41.29989058213949,-72.92383378813902,114.0));
console.log("should be 1.0:", addon.predict(0.0,0.0,1.0,41.29910840804458,-72.92590113638599,24.5876502990723,41.308917976337376,-72.91900562948886,485.0));
console.log("should be 0.0:", addon.predict(0.0,0.0,1.0,32.71620476049558,-117.16931190249628,3.03776454925537,32.71780755891966,-117.1701536138048,129.0));
console.log("should be 1.0:", addon.predict(0.0,0.0,1.0,32.71620476049558,-117.16931190249628,0.0111846998333931,32.75424698714334,-117.19953872934138,486.0));
console.log("should be 1.0:", addon.predict(0.0,0.0,1.0,31.790226055804904,-90.40704924636631,0.0,31.790275708885,-90.40701115905895,288.0));
