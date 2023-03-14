static const int th_len[] = {
  44, 8, 6, 28, 39, 49, 18, 11, 30, 
};
static const int th_begin[] = {
  0, 44, 52, 58, 86, 125, 174, 192, 203, 
};
static const float threshold[] = {
  0.0111847, 0.018641129, 0.0335541, 0.037282258, 0.055923391, 0.069345139, 
  0.091714539, 0.10289924, 0.88582826, 1.068758, 1.4853282, 3.6785159, 
  4.062283, 4.3374267, 5.0455322, 5.8471012, 6.6424561, 7.9635062, 8.6144562, 
  13.716916, 13.95599, 14.913679, 16.46633, 17.076799, 33.641029, 35.24416, 
  38.580921, 40.42598, 43.092079, 43.533249, 44.471519, 45.962406, 52.667412, 
  53.767231, 54.362118, 55.10939, 56.813801, 58.39645, 62.727402, 78.908058, 
  79.610458, 99.944244, 106.52756, 111.5609, -0.84707773, -0.36492836, 
  -0.1312952, 0, 0.48400983, 0.76996553, 0.98260766, 1.9652253, -0.87506258, 
  -0.31309801, -0.18569385, 0.53146899, 0.99134332, 1, 29.161036, 33.476738, 
  34.062122, 37.292614, 37.534641, 37.61684, 37.670162, 37.793903, 38.910946, 
  39.297817, 39.307476, 39.332897, 39.610687, 40.717049, 40.836647, 41.093998, 
  41.299107, 41.778168, 41.879009, 42.171402, 42.345955, 42.438072, 42.642166, 
  42.90741, 45.528507, 45.654213, 46.721344, 47.596649, -122.59035, -122.36012, 
  -122.29141, -122.27431, -122.03381, -121.24682, -120.65991, -120.12331, 
  -118.23631, -118.22995, -91.011398, -88.739876, -87.865944, -87.637085, 
  -87.590881, -86.868423, -83.246956, -81.956757, -81.701538, -79.941811, 
  -79.787018, -79.007362, -78.908646, -78.277847, -77.772209, -77.488457, 
  -77.431664, -77.317101, -75.551575, -75.250771, -75.182854, -75.181679, 
  -75.181267, -74.190887, -73.996178, -73.934158, -73.877762, -72.625992, 
  -71.058846, 0.0223694, 0.0335541, 0.06576886, 0.11976302, 0.18603203, 
  0.22746357, 0.29974997, 0.37312704, 0.56019735, 1.1437156, 1.2998655, 
  1.6073903, 2.1527088, 2.6713843, 3.0010297, 3.5869567, 5.7623801, 8.1458464, 
  8.5065527, 9.9274254, 10.0165, 10.40511, 10.761724, 11.72604, 12.381463, 
  13.50239, 13.807195, 22.04003, 22.779461, 23.531321, 24.295601, 24.81756, 
  25.249369, 29.22308, 30.679632, 36.72924, 37.897419, 38.717628, 44.011711, 
  51.818714, 61.75185, 80.417831, 88.048271, 89.289696, 102.91143, 108, 117.9, 
  122.9, 126.46201, 34.061954, 37.337917, 38.266262, 39.96331, 39.990219, 
  40.499863, 40.752464, 41.874702, 41.879009, 41.879028, 41.879059, 42.10601, 
  42.106537, 42.345993, 42.348572, 42.348919, 42.64222, 42.643005, -122.67643, 
  -87.924286, -87.638771, -79.161125, -77.481491, -77.437569, -75.182465, 
  -74.117157, -72.925728, -72.922577, -71.059059, 11, 22, 31, 45, 79, 109, 120, 
  130, 146, 154, 163, 170, 176, 181, 190, 195, 208, 240, 301, 316, 347, 357, 
  424, 527, 530, 551, 557, 579, 581, 596, 
};

#include <stdlib.h>

/*
 * \brief function to convert a feature value into bin index.
 * \param val feature value, in floating-point
 * \param fid feature identifier
 * \return bin index corresponding to given feature value
 */
static inline int quantize(float val, unsigned fid) {
  const size_t offset = th_begin[fid];
  const float* array = &threshold[offset];
  int len = th_len[fid];
  int low = 0;
  int high = len;
  int mid;
  float mval;
  // It is possible th_begin[i] == [total_num_threshold]. This means that
  // all features i, (i+1), ... are not used for any of the splits in the model.
  // So in this case, just return something
  if (offset == 233 || val < array[0]) {
    return -10;
  }
  while (low + 1 < high) {
    mid = (low + high) / 2;
    mval = array[mid];
    if (val == mval) {
      return mid * 2;
    } else if (val < mval) {
      high = mid;
    } else {
      low = mid;
    }
  }
  if (array[low] == val) {
    return low * 2;
  } else if (high == len) {
    return len * 2;
  } else {
    return low * 2 + 1;
  }
}

#include "header.h"

const unsigned char is_categorical[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 
};


size_t get_num_class(void) {
  return 1;
}

size_t get_num_feature(void) {
  return 9;
}

const char* get_pred_transform(void) {
  return "hinge";
}

float get_sigmoid_alpha(void) {
  return 1.0;
}

float get_global_bias(void) {
  return 0.5;
}

const char* get_threshold_type(void) {
  return "float32";
}

const char* get_leaf_output_type(void) {
  return "float32";
}


static inline float pred_transform(float margin) {
  if (margin > 0) {
    return (float)(1);
  } else {
    return (float)(0);
  }
}
float predict(union Entry* data, int pred_margin) {

  for (int i = 0; i < 9; ++i) {
    if (data[i].missing != -1 && !is_categorical[i]) {
      data[i].qvalue = quantize(data[i].fvalue, i);
    }
  }
  float sum = (float)0;
  unsigned int tmp;
  int nid, cond, fid;  /* used for folded subtrees */
  if ( (data[0].missing != -1) && (data[0].qvalue < 28)) {
    if ( (data[5].missing != -1) && (data[5].qvalue < 48)) {
      if ( (data[5].missing != -1) && (data[5].qvalue < 6)) {
        if ( (data[8].missing != -1) && (data[8].qvalue < 14)) {
          if ( (data[4].missing != -1) && (data[4].qvalue < 26)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 0)) {
              sum += (float)0.25;
            } else {
              sum += (float)-0.20000001788;
            }
          } else {
            sum += (float)-0.28125;
          }
        } else {
          if ( (data[5].missing != -1) && (data[5].qvalue < 4)) {
            if ( (data[2].missing != -1) && (data[2].qvalue < 10)) {
              sum += (float)0.28932806849;
            } else {
              sum += (float)0.23736265302;
            }
          } else {
            if ( (data[2].missing != -1) && (data[2].qvalue < 10)) {
              sum += (float)0.23181818426;
            } else {
              sum += (float)-0.22500000894;
            }
          }
        }
      } else {
        if ( (data[6].missing != -1) && (data[6].qvalue < 34)) {
          if ( (data[6].missing != -1) && (data[6].qvalue < 4)) {
            if ( (data[6].missing != -1) && (data[6].qvalue < 0)) {
              sum += (float)-0.26280000806;
            } else {
              sum += (float)-0.11902174354;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 30)) {
              sum += (float)-0.29366198182;
            } else {
              sum += (float)-0.26049724221;
            }
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 10)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 34)) {
              sum += (float)0.17647060752;
            } else {
              sum += (float)0.28999999166;
            }
          } else {
            if ( (data[5].missing != -1) && (data[5].qvalue < 18)) {
              sum += (float)0.25;
            } else {
              sum += (float)-0.20000001788;
            }
          }
        }
      }
    } else {
      if ( (data[5].missing != -1) && (data[5].qvalue < 58)) {
        if ( (data[4].missing != -1) && (data[4].qvalue < 54)) {
          if ( (data[4].missing != -1) && (data[4].qvalue < 4)) {
            if ( (data[1].missing != -1) && (data[1].qvalue < 8)) {
              sum += (float)-0.24000000954;
            } else {
              sum += (float)0.15000000596;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 0)) {
              sum += (float)-0.22500000894;
            } else {
              sum += (float)0.20975610614;
            }
          }
        } else {
          if ( (data[6].missing != -1) && (data[6].qvalue < 32)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 16)) {
              sum += (float)0.035294119269;
            } else {
              sum += (float)-0.19726027548;
            }
          } else {
            sum += (float)0.25;
          }
        }
      } else {
        if ( (data[8].missing != -1) && (data[8].qvalue < 18)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 16)) {
            sum += (float)-0.27692309022;
          } else {
            sum += (float)0.24000000954;
          }
        } else {
          if ( (data[7].missing != -1) && (data[7].qvalue < 12)) {
            if ( (data[1].missing != -1) && (data[1].qvalue < 14)) {
              sum += (float)0.28125;
            } else {
              sum += (float)-0.15000000596;
            }
          } else {
            if ( (data[6].missing != -1) && (data[6].qvalue < 6)) {
              sum += (float)-0.26250001788;
            } else {
              sum += (float)0.20645162463;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].missing != -1) && (data[8].qvalue < 10)) {
      if ( (data[3].missing != -1) && (data[3].qvalue < 6)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 40)) {
          sum += (float)0.27272728086;
        } else {
          sum += (float)-0.20000001788;
        }
      } else {
        if ( (data[4].missing != -1) && (data[4].qvalue < 12)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 78)) {
            if ( (data[1].missing != -1) && (data[1].qvalue < 4)) {
              sum += (float)-0.24000000954;
            } else {
              sum += (float)0.15000000596;
            }
          } else {
            sum += (float)0.22500000894;
          }
        } else {
          if ( (data[8].missing != -1) && (data[8].qvalue < 8)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 86)) {
              sum += (float)-0.26250001788;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 32)) {
              sum += (float)0.24000000954;
            } else {
              sum += (float)-0.21818183362;
            }
          }
        }
      }
    } else {
      if ( (data[4].missing != -1) && (data[4].qvalue < 54)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 46)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 60)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 10)) {
              sum += (float)0.29010990262;
            } else {
              sum += (float)0.063333339989;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 52)) {
              sum += (float)0.27985075116;
            } else {
              sum += (float)0.15000000596;
            }
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 48)) {
            sum += (float)0.28907895088;
          } else {
            if ( (data[2].missing != -1) && (data[2].qvalue < 10)) {
              sum += (float)0.26666668057;
            } else {
              sum += (float)0.03750000149;
            }
          }
        }
      } else {
        if ( (data[3].missing != -1) && (data[3].qvalue < 28)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 8)) {
            sum += (float)0.29808917642;
          } else {
            if ( (data[7].missing != -1) && (data[7].qvalue < 12)) {
              sum += (float)0.17057523131;
            } else {
              sum += (float)0.0031690141186;
            }
          }
        } else {
          if ( (data[0].missing != -1) && (data[0].qvalue < 34)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 30)) {
              sum += (float)0.25714287162;
            } else {
              sum += (float)-0.25;
            }
          } else {
            if ( (data[2].missing != -1) && (data[2].qvalue < 0)) {
              sum += (float)0.21000000834;
            } else {
              sum += (float)0.27739307284;
            }
          }
        }
      }
    }
  }
  if ( (data[0].missing != -1) && (data[0].qvalue < 28)) {
    if ( (data[5].missing != -1) && (data[5].qvalue < 48)) {
      if ( (data[5].missing != -1) && (data[5].qvalue < 6)) {
        if ( (data[8].missing != -1) && (data[8].qvalue < 14)) {
          if ( (data[4].missing != -1) && (data[4].qvalue < 26)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 0)) {
              sum += (float)0.25;
            } else {
              sum += (float)-0.20000001788;
            }
          } else {
            sum += (float)-0.28125;
          }
        } else {
          if ( (data[5].missing != -1) && (data[5].qvalue < 4)) {
            if ( (data[2].missing != -1) && (data[2].qvalue < 10)) {
              sum += (float)0.28932806849;
            } else {
              sum += (float)0.23736265302;
            }
          } else {
            if ( (data[2].missing != -1) && (data[2].qvalue < 10)) {
              sum += (float)0.23181818426;
            } else {
              sum += (float)-0.22500000894;
            }
          }
        }
      } else {
        if ( (data[6].missing != -1) && (data[6].qvalue < 34)) {
          if ( (data[6].missing != -1) && (data[6].qvalue < 4)) {
            if ( (data[6].missing != -1) && (data[6].qvalue < 0)) {
              sum += (float)-0.26280000806;
            } else {
              sum += (float)-0.11902174354;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 30)) {
              sum += (float)-0.29366198182;
            } else {
              sum += (float)-0.26049724221;
            }
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 10)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 34)) {
              sum += (float)0.17647060752;
            } else {
              sum += (float)0.28999999166;
            }
          } else {
            if ( (data[5].missing != -1) && (data[5].qvalue < 18)) {
              sum += (float)0.25;
            } else {
              sum += (float)-0.20000001788;
            }
          }
        }
      }
    } else {
      if ( (data[5].missing != -1) && (data[5].qvalue < 58)) {
        if ( (data[4].missing != -1) && (data[4].qvalue < 54)) {
          if ( (data[4].missing != -1) && (data[4].qvalue < 4)) {
            if ( (data[1].missing != -1) && (data[1].qvalue < 8)) {
              sum += (float)-0.24000000954;
            } else {
              sum += (float)0.15000000596;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 0)) {
              sum += (float)-0.22500000894;
            } else {
              sum += (float)0.20975610614;
            }
          }
        } else {
          if ( (data[6].missing != -1) && (data[6].qvalue < 32)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 16)) {
              sum += (float)0.035294119269;
            } else {
              sum += (float)-0.19726027548;
            }
          } else {
            sum += (float)0.25;
          }
        }
      } else {
        if ( (data[8].missing != -1) && (data[8].qvalue < 18)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 16)) {
            sum += (float)-0.27692309022;
          } else {
            sum += (float)0.24000000954;
          }
        } else {
          if ( (data[7].missing != -1) && (data[7].qvalue < 12)) {
            if ( (data[1].missing != -1) && (data[1].qvalue < 14)) {
              sum += (float)0.28125;
            } else {
              sum += (float)-0.15000000596;
            }
          } else {
            if ( (data[6].missing != -1) && (data[6].qvalue < 6)) {
              sum += (float)-0.26250001788;
            } else {
              sum += (float)0.20645162463;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].missing != -1) && (data[8].qvalue < 10)) {
      if ( (data[3].missing != -1) && (data[3].qvalue < 6)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 40)) {
          sum += (float)0.27272728086;
        } else {
          sum += (float)-0.20000001788;
        }
      } else {
        if ( (data[4].missing != -1) && (data[4].qvalue < 12)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 78)) {
            if ( (data[1].missing != -1) && (data[1].qvalue < 4)) {
              sum += (float)-0.24000000954;
            } else {
              sum += (float)0.15000000596;
            }
          } else {
            sum += (float)0.22500000894;
          }
        } else {
          if ( (data[8].missing != -1) && (data[8].qvalue < 8)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 86)) {
              sum += (float)-0.26250001788;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 32)) {
              sum += (float)0.24000000954;
            } else {
              sum += (float)-0.21818183362;
            }
          }
        }
      }
    } else {
      if ( (data[4].missing != -1) && (data[4].qvalue < 54)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 46)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 60)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 10)) {
              sum += (float)0.29010990262;
            } else {
              sum += (float)0.063333339989;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 52)) {
              sum += (float)0.27985075116;
            } else {
              sum += (float)0.15000000596;
            }
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 48)) {
            sum += (float)0.28907895088;
          } else {
            if ( (data[2].missing != -1) && (data[2].qvalue < 10)) {
              sum += (float)0.26666668057;
            } else {
              sum += (float)0.03750000149;
            }
          }
        }
      } else {
        if ( (data[3].missing != -1) && (data[3].qvalue < 28)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 8)) {
            sum += (float)0.29808917642;
          } else {
            if ( (data[7].missing != -1) && (data[7].qvalue < 12)) {
              sum += (float)0.17057523131;
            } else {
              sum += (float)0.0031690141186;
            }
          }
        } else {
          if ( (data[0].missing != -1) && (data[0].qvalue < 34)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 30)) {
              sum += (float)0.25714287162;
            } else {
              sum += (float)-0.25;
            }
          } else {
            if ( (data[2].missing != -1) && (data[2].qvalue < 0)) {
              sum += (float)0.21000000834;
            } else {
              sum += (float)0.27739307284;
            }
          }
        }
      }
    }
  }
  if ( (data[5].missing != -1) && (data[5].qvalue < 48)) {
    if ( (data[5].missing != -1) && (data[5].qvalue < 6)) {
      if ( (data[8].missing != -1) && (data[8].qvalue < 14)) {
        sum += (float)-0.28499999642;
      } else {
        if ( (data[5].missing != -1) && (data[5].qvalue < 0)) {
          if ( (data[1].missing != -1) && (data[1].qvalue < 0)) {
            sum += (float)-0.15000000596;
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 26)) {
              sum += (float)0.10909091681;
            } else {
              sum += (float)0.27887326479;
            }
          }
        } else {
          if ( (data[0].missing != -1) && (data[0].qvalue < 0)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 24)) {
              sum += (float)0.17142859101;
            } else {
              sum += (float)-0.28125;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 14)) {
              sum += (float)0.26785716414;
            } else {
              sum += (float)-0.20000001788;
            }
          }
        }
      }
    } else {
      if ( (data[0].missing != -1) && (data[0].qvalue < 36)) {
        if ( (data[4].missing != -1) && (data[4].qvalue < 16)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 20)) {
            if ( (data[3].missing != -1) && (data[3].qvalue < 14)) {
              sum += (float)0.23571428657;
            } else {
              sum += (float)0.018750000745;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 54)) {
              sum += (float)-0.20139861107;
            } else {
              sum += (float)0.26250001788;
            }
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 54)) {
            if ( (data[7].missing != -1) && (data[7].qvalue < 4)) {
              sum += (float)-0.27896440029;
            } else {
              sum += (float)-0.057236846536;
            }
          } else {
            if ( (data[7].missing != -1) && (data[7].qvalue < 16)) {
              sum += (float)-0.29406040907;
            } else {
              sum += (float)-0.25403898954;
            }
          }
        }
      } else {
        if ( (data[5].missing != -1) && (data[5].qvalue < 38)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 48)) {
            if ( (data[6].missing != -1) && (data[6].qvalue < 30)) {
              sum += (float)-0.21935485303;
            } else {
              sum += (float)0.10909091681;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 22)) {
              sum += (float)-0.24000000954;
            } else {
              sum += (float)0.19615384936;
            }
          }
        } else {
          if ( (data[2].missing != -1) && (data[2].qvalue < 8)) {
            if ( (data[2].missing != -1) && (data[2].qvalue < 2)) {
              sum += (float)0.05000000447;
            } else {
              sum += (float)0.28636363149;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 50)) {
              sum += (float)-0.20000001788;
            } else {
              sum += (float)0.15000000596;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].missing != -1) && (data[8].qvalue < 10)) {
      if ( (data[4].missing != -1) && (data[4].qvalue < 12)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 46)) {
          sum += (float)0.25;
        } else {
          if ( (data[1].missing != -1) && (data[1].qvalue < 4)) {
            sum += (float)-0.22500000894;
          } else {
            sum += (float)0.15000000596;
          }
        }
      } else {
        if ( (data[8].missing != -1) && (data[8].qvalue < 8)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 78)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 70)) {
              sum += (float)-0.27794119716;
            } else {
              sum += (float)-0.11250000447;
            }
          } else {
            if ( (data[1].missing != -1) && (data[1].qvalue < 4)) {
              sum += (float)-0.20000001788;
            } else {
              sum += (float)0.20000001788;
            }
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 32)) {
            sum += (float)0.24000000954;
          } else {
            if ( (data[1].missing != -1) && (data[1].qvalue < 0)) {
              sum += (float)0.15000000596;
            } else {
              sum += (float)-0.27500000596;
            }
          }
        }
      }
    } else {
      if ( (data[5].missing != -1) && (data[5].qvalue < 74)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 54)) {
          if ( (data[4].missing != -1) && (data[4].qvalue < 54)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 58)) {
              sum += (float)0.16050955653;
            } else {
              sum += (float)-0.26399999857;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 16)) {
              sum += (float)0.16000001132;
            } else {
              sum += (float)-0.10544555634;
            }
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 52)) {
            sum += (float)-0.20000001788;
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 12)) {
              sum += (float)-0.15000000596;
            } else {
              sum += (float)0.2650000155;
            }
          }
        }
      } else {
        if ( (data[4].missing != -1) && (data[4].qvalue < 52)) {
          if ( (data[3].missing != -1) && (data[3].qvalue < 48)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 16)) {
              sum += (float)0.07500000298;
            } else {
              sum += (float)-0.27352941036;
            }
          } else {
            sum += (float)0.22500000894;
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 64)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 96)) {
              sum += (float)0.24682274461;
            } else {
              sum += (float)-0.085714295506;
            }
          } else {
            if ( (data[2].missing != -1) && (data[2].qvalue < 0)) {
              sum += (float)0.25243902206;
            } else {
              sum += (float)0.034800000489;
            }
          }
        }
      }
    }
  }
  if ( (data[5].missing != -1) && (data[5].qvalue < 50)) {
    if ( (data[0].missing != -1) && (data[0].qvalue < 34)) {
      if ( (data[5].missing != -1) && (data[5].qvalue < 2)) {
        if ( (data[2].missing != -1) && (data[2].qvalue < 8)) {
          if ( (data[8].missing != -1) && (data[8].qvalue < 14)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 0)) {
              sum += (float)0.15000000596;
            } else {
              sum += (float)-0.15000000596;
            }
          } else {
            sum += (float)0.27272728086;
          }
        } else {
          if ( (data[6].missing != -1) && (data[6].qvalue < 18)) {
            sum += (float)-0.27272728086;
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 0)) {
              sum += (float)0.24000000954;
            } else {
              sum += (float)-0.15000000596;
            }
          }
        }
      } else {
        if ( (data[6].missing != -1) && (data[6].qvalue < 34)) {
          if ( (data[6].missing != -1) && (data[6].qvalue < 4)) {
            if ( (data[6].missing != -1) && (data[6].qvalue < 0)) {
              sum += (float)-0.25898438692;
            } else {
              sum += (float)-0.11907216907;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 32)) {
              sum += (float)-0.29290574789;
            } else {
              sum += (float)-0.25807985663;
            }
          }
        } else {
          if ( (data[5].missing != -1) && (data[5].qvalue < 18)) {
            if ( (data[2].missing != -1) && (data[2].qvalue < 8)) {
              sum += (float)0.28571429849;
            } else {
              sum += (float)0.12000000477;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 10)) {
              sum += (float)0.17647060752;
            } else {
              sum += (float)-0.18409091234;
            }
          }
        }
      }
    } else {
      if ( (data[5].missing != -1) && (data[5].qvalue < 8)) {
        if ( (data[8].missing != -1) && (data[8].qvalue < 4)) {
          sum += (float)-0.15000000596;
        } else {
          sum += (float)0.29845362902;
        }
      } else {
        if ( (data[7].missing != -1) && (data[7].qvalue < 6)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 36)) {
            if ( (data[2].missing != -1) && (data[2].qvalue < 6)) {
              sum += (float)-0.15000000596;
            } else {
              sum += (float)0.16500000656;
            }
          } else {
            if ( (data[1].missing != -1) && (data[1].qvalue < 10)) {
              sum += (float)0.15789474547;
            } else {
              sum += (float)0.28636363149;
            }
          }
        } else {
          if ( (data[5].missing != -1) && (data[5].qvalue < 38)) {
            if ( (data[1].missing != -1) && (data[1].qvalue < 0)) {
              sum += (float)0.025000002235;
            } else {
              sum += (float)-0.22025318444;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 12)) {
              sum += (float)-0.22500000894;
            } else {
              sum += (float)0.20000001788;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].missing != -1) && (data[4].qvalue < 54)) {
      if ( (data[8].missing != -1) && (data[8].qvalue < 10)) {
        if ( (data[5].missing != -1) && (data[5].qvalue < 52)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 8)) {
            sum += (float)-0.15000000596;
          } else {
            sum += (float)0.24000000954;
          }
        } else {
          if ( (data[3].missing != -1) && (data[3].qvalue < 0)) {
            sum += (float)0.15000000596;
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 28)) {
              sum += (float)-0.29062500596;
            } else {
              sum += (float)-0.17142859101;
            }
          }
        }
      } else {
        if ( (data[5].missing != -1) && (data[5].qvalue < 56)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 38)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 50)) {
              sum += (float)-0.21290323138;
            } else {
              sum += (float)0.22500000894;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 44)) {
              sum += (float)0.12000000477;
            } else {
              sum += (float)0.28363636136;
            }
          }
        } else {
          if ( (data[1].missing != -1) && (data[1].qvalue < 14)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 18)) {
              sum += (float)0.16875000298;
            } else {
              sum += (float)0.28419527411;
            }
          } else {
            sum += (float)-0.15000000596;
          }
        }
      }
    } else {
      if ( (data[6].missing != -1) && (data[6].qvalue < 8)) {
        if ( (data[5].missing != -1) && (data[5].qvalue < 66)) {
          if ( (data[3].missing != -1) && (data[3].qvalue < 16)) {
            if (!(data[1].missing != -1) || (data[1].qvalue < 8)) {
              sum += (float)-0.25714287162;
            } else {
              sum += (float)0.13333334029;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 20)) {
              sum += (float)0.1894736886;
            } else {
              sum += (float)-0.05000000447;
            }
          }
        } else {
          if ( (data[0].missing != -1) && (data[0].qvalue < 42)) {
            if ( (data[2].missing != -1) && (data[2].qvalue < 2)) {
              sum += (float)-0.22500000894;
            } else {
              sum += (float)0.15000000596;
            }
          } else {
            if ( (data[5].missing != -1) && (data[5].qvalue < 92)) {
              sum += (float)-0.28571429849;
            } else {
              sum += (float)-0.066666670144;
            }
          }
        }
      } else {
        if ( (data[8].missing != -1) && (data[8].qvalue < 10)) {
          if ( (data[1].missing != -1) && (data[1].qvalue < 12)) {
            if ( (data[1].missing != -1) && (data[1].qvalue < 4)) {
              sum += (float)-0.16000001132;
            } else {
              sum += (float)-0.2842105329;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 32)) {
              sum += (float)-0.15000000596;
            } else {
              sum += (float)0.20000001788;
            }
          }
        } else {
          if ( (data[0].missing != -1) && (data[0].qvalue < 74)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 76)) {
              sum += (float)0.13625499606;
            } else {
              sum += (float)-0.10588236153;
            }
          } else {
            if ( (data[2].missing != -1) && (data[2].qvalue < 8)) {
              sum += (float)-0.099107146263;
            } else {
              sum += (float)0.23076924682;
            }
          }
        }
      }
    }
  }
  if ( (data[5].missing != -1) && (data[5].qvalue < 48)) {
    if ( (data[5].missing != -1) && (data[5].qvalue < 0)) {
      if ( (data[4].missing != -1) && (data[4].qvalue < 68)) {
        if ( (data[1].missing != -1) && (data[1].qvalue < 0)) {
          sum += (float)-0.15000000596;
        } else {
          if ( (data[8].missing != -1) && (data[8].qvalue < 26)) {
            if ( (data[3].missing != -1) && (data[3].qvalue < 4)) {
              sum += (float)-0.22500000894;
            } else {
              sum += (float)0.20000001788;
            }
          } else {
            if ( (data[6].missing != -1) && (data[6].qvalue < 14)) {
              sum += (float)0.29361703992;
            } else {
              sum += (float)-0.20000001788;
            }
          }
        }
      } else {
        sum += (float)-0.20000001788;
      }
    } else {
      if ( (data[0].missing != -1) && (data[0].qvalue < 24)) {
        if ( (data[4].missing != -1) && (data[4].qvalue < 54)) {
          if ( (data[7].missing != -1) && (data[7].qvalue < 4)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 36)) {
              sum += (float)-0.27798467875;
            } else {
              sum += (float)-0.15989011526;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 26)) {
              sum += (float)0.11470589042;
            } else {
              sum += (float)-0.10982143134;
            }
          }
        } else {
          if ( (data[5].missing != -1) && (data[5].qvalue < 4)) {
            if ( (data[3].missing != -1) && (data[3].qvalue < 32)) {
              sum += (float)-0.27500000596;
            } else {
              sum += (float)0.26666668057;
            }
          } else {
            if ( (data[7].missing != -1) && (data[7].qvalue < 16)) {
              sum += (float)-0.29519918561;
            } else {
              sum += (float)-0.25365167856;
            }
          }
        }
      } else {
        if ( (data[0].missing != -1) && (data[0].qvalue < 50)) {
          if ( (data[4].missing != -1) && (data[4].qvalue < 34)) {
            if ( (data[2].missing != -1) && (data[2].qvalue < 8)) {
              sum += (float)-0.10833333433;
            } else {
              sum += (float)0.11428572237;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 26)) {
              sum += (float)-0.063157901168;
            } else {
              sum += (float)-0.22164179385;
            }
          }
        } else {
          if ( (data[8].missing != -1) && (data[8].qvalue < 22)) {
            sum += (float)-0.24000000954;
          } else {
            if ( (data[5].missing != -1) && (data[5].qvalue < 36)) {
              sum += (float)0.23478262126;
            } else {
              sum += (float)-0.07500000298;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].missing != -1) && (data[8].qvalue < 10)) {
      if ( (data[4].missing != -1) && (data[4].qvalue < 12)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 46)) {
          sum += (float)0.25;
        } else {
          if ( (data[1].missing != -1) && (data[1].qvalue < 4)) {
            sum += (float)-0.22500000894;
          } else {
            sum += (float)0.15000000596;
          }
        }
      } else {
        if ( (data[8].missing != -1) && (data[8].qvalue < 8)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 78)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 70)) {
              sum += (float)-0.27794119716;
            } else {
              sum += (float)-0.11250000447;
            }
          } else {
            if ( (data[1].missing != -1) && (data[1].qvalue < 4)) {
              sum += (float)-0.20000001788;
            } else {
              sum += (float)0.20000001788;
            }
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 38)) {
            sum += (float)0.24000000954;
          } else {
            if ( (data[1].missing != -1) && (data[1].qvalue < 0)) {
              sum += (float)0.15000000596;
            } else {
              sum += (float)-0.27500000596;
            }
          }
        }
      }
    } else {
      if ( (data[0].missing != -1) && (data[0].qvalue < 60)) {
        if (!(data[2].missing != -1) || (data[2].qvalue < 0)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 84)) {
            if ( (data[3].missing != -1) && (data[3].qvalue < 10)) {
              sum += (float)0.13846154511;
            } else {
              sum += (float)-0.16621622443;
            }
          } else {
            sum += (float)0.26250001788;
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 40)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 56)) {
              sum += (float)0.19626168907;
            } else {
              sum += (float)-0.12972973287;
            }
          } else {
            if ( (data[5].missing != -1) && (data[5].qvalue < 76)) {
              sum += (float)-0.040277779102;
            } else {
              sum += (float)0.11200000346;
            }
          }
        }
      } else {
        if ( (data[7].missing != -1) && (data[7].qvalue < 10)) {
          sum += (float)-0.2842105329;
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 58)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 92)) {
              sum += (float)0.24500000477;
            } else {
              sum += (float)-0.11250000447;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 26)) {
              sum += (float)0.010650888085;
            } else {
              sum += (float)0.20113638043;
            }
          }
        }
      }
    }
  }
  if ( (data[5].missing != -1) && (data[5].qvalue < 50)) {
    if ( (data[5].missing != -1) && (data[5].qvalue < 8)) {
      if ( (data[0].missing != -1) && (data[0].qvalue < 0)) {
        if ( (data[4].missing != -1) && (data[4].qvalue < 14)) {
          sum += (float)0.22500000894;
        } else {
          if ( (data[2].missing != -1) && (data[2].qvalue < 8)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 2)) {
              sum += (float)0.17142859101;
            } else {
              sum += (float)-0.25;
            }
          } else {
            sum += (float)-0.29454547167;
          }
        }
      } else {
        if ( (data[8].missing != -1) && (data[8].qvalue < 16)) {
          sum += (float)-0.26666668057;
        } else {
          if ( (data[3].missing != -1) && (data[3].qvalue < 2)) {
            sum += (float)-0.15000000596;
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 6)) {
              sum += (float)0.20000001788;
            } else {
              sum += (float)0.28392860293;
            }
          }
        }
      }
    } else {
      if ( (data[0].missing != -1) && (data[0].qvalue < 24)) {
        if ( (data[6].missing != -1) && (data[6].qvalue < 34)) {
          if ( (data[6].missing != -1) && (data[6].qvalue < 4)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 36)) {
              sum += (float)-0.23236076534;
            } else {
              sum += (float)-0.011764707044;
            }
          } else {
            if ( (data[5].missing != -1) && (data[5].qvalue < 44)) {
              sum += (float)-0.28734210134;
            } else {
              sum += (float)-0.14285714924;
            }
          }
        } else {
          if ( (data[5].missing != -1) && (data[5].qvalue < 18)) {
            if ( (data[3].missing != -1) && (data[3].qvalue < 50)) {
              sum += (float)0.27692309022;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 10)) {
              sum += (float)0.16000001132;
            } else {
              sum += (float)-0.18157896399;
            }
          }
        }
      } else {
        if ( (data[7].missing != -1) && (data[7].qvalue < 6)) {
          if ( (data[8].missing != -1) && (data[8].qvalue < 32)) {
            if ( (data[3].missing != -1) && (data[3].qvalue < 6)) {
              sum += (float)0.090000003576;
            } else {
              sum += (float)-0.21176472306;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 40)) {
              sum += (float)0.28000000119;
            } else {
              sum += (float)-0.036363638937;
            }
          }
        } else {
          if ( (data[0].missing != -1) && (data[0].qvalue < 62)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 40)) {
              sum += (float)-0.20425532758;
            } else {
              sum += (float)-0.024000000209;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 22)) {
              sum += (float)-0.22500000894;
            } else {
              sum += (float)0.26250001788;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].missing != -1) && (data[7].qvalue < 8)) {
      if ( (data[8].missing != -1) && (data[8].qvalue < 10)) {
        if ( (data[5].missing != -1) && (data[5].qvalue < 52)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 8)) {
            sum += (float)-0.15000000596;
          } else {
            sum += (float)0.24000000954;
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 0)) {
            sum += (float)0.15000000596;
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 28)) {
              sum += (float)-0.29062500596;
            } else {
              sum += (float)-0.15000000596;
            }
          }
        }
      } else {
        if ( (data[5].missing != -1) && (data[5].qvalue < 56)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 44)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 42)) {
              sum += (float)-0.17142859101;
            } else {
              sum += (float)0.125;
            }
          } else {
            sum += (float)0.27954545617;
          }
        } else {
          if ( (data[1].missing != -1) && (data[1].qvalue < 14)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 18)) {
              sum += (float)0.16000001132;
            } else {
              sum += (float)0.28393137455;
            }
          } else {
            sum += (float)-0.15000000596;
          }
        }
      }
    } else {
      if ( (data[8].missing != -1) && (data[8].qvalue < 10)) {
        if ( (data[3].missing != -1) && (data[3].qvalue < 42)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 86)) {
            if ( (data[2].missing != -1) && (data[2].qvalue < 4)) {
              sum += (float)-0.18333333731;
            } else {
              sum += (float)-0.29117646813;
            }
          } else {
            if ( (data[1].missing != -1) && (data[1].qvalue < 4)) {
              sum += (float)-0.15000000596;
            } else {
              sum += (float)0.15000000596;
            }
          }
        } else {
          sum += (float)0.15000000596;
        }
      } else {
        if ( (data[5].missing != -1) && (data[5].qvalue < 90)) {
          if ( (data[0].missing != -1) && (data[0].qvalue < 82)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 74)) {
              sum += (float)-0.016242038459;
            } else {
              sum += (float)-0.15000000596;
            }
          } else {
            if ( (data[5].missing != -1) && (data[5].qvalue < 88)) {
              sum += (float)0.24000000954;
            } else {
              sum += (float)-0.25;
            }
          }
        } else {
          if ( (data[1].missing != -1) && (data[1].qvalue < 0)) {
            if ( (data[3].missing != -1) && (data[3].qvalue < 24)) {
              sum += (float)0.22500000894;
            } else {
              sum += (float)-0.22500000894;
            }
          } else {
            if ( (data[1].missing != -1) && (data[1].qvalue < 10)) {
              sum += (float)0.28636363149;
            } else {
              sum += (float)-0.062068969011;
            }
          }
        }
      }
    }
  }
  if ( (data[5].missing != -1) && (data[5].qvalue < 22)) {
    if ( (data[8].missing != -1) && (data[8].qvalue < 4)) {
      if ( (data[3].missing != -1) && (data[3].qvalue < 6)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 10)) {
          sum += (float)-0.15000000596;
        } else {
          sum += (float)0.15000000596;
        }
      } else {
        sum += (float)-0.25714287162;
      }
    } else {
      if ( (data[1].missing != -1) && (data[1].qvalue < 6)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 4)) {
          if ( (data[3].missing != -1) && (data[3].qvalue < 22)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 0)) {
              sum += (float)-0.26250001788;
            } else {
              sum += (float)-0.05000000447;
            }
          } else {
            if ( (data[1].missing != -1) && (data[1].qvalue < 0)) {
              sum += (float)0.20000001788;
            } else {
              sum += (float)-0.07500000298;
            }
          }
        } else {
          if ( (data[8].missing != -1) && (data[8].qvalue < 16)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 12)) {
              sum += (float)0.15000000596;
            } else {
              sum += (float)-0.24000000954;
            }
          } else {
            if ( (data[5].missing != -1) && (data[5].qvalue < 14)) {
              sum += (float)0.21250000596;
            } else {
              sum += (float)0.062068969011;
            }
          }
        }
      } else {
        if ( (data[0].missing != -1) && (data[0].qvalue < 18)) {
          if ( (data[8].missing != -1) && (data[8].qvalue < 52)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 18)) {
              sum += (float)0.10500000417;
            } else {
              sum += (float)0.22500000894;
            }
          } else {
            sum += (float)-0.15000000596;
          }
        } else {
          if ( (data[1].missing != -1) && (data[1].qvalue < 10)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 26)) {
              sum += (float)0.15000000596;
            } else {
              sum += (float)-0.1875;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 20)) {
              sum += (float)-0.15000000596;
            } else {
              sum += (float)0.25714287162;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].missing != -1) && (data[5].qvalue < 40)) {
      if ( (data[2].missing != -1) && (data[2].qvalue < 10)) {
        if ( (data[4].missing != -1) && (data[4].qvalue < 22)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 24)) {
            if ( (data[1].missing != -1) && (data[1].qvalue < 4)) {
              sum += (float)-0.12000000477;
            } else {
              sum += (float)0.22500000894;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 2)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.25096154213;
            }
          }
        } else {
          if ( (data[1].missing != -1) && (data[1].qvalue < 2)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 46)) {
              sum += (float)0.16000001132;
            } else {
              sum += (float)-0.070588238537;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 26)) {
              sum += (float)0.090000003576;
            } else {
              sum += (float)-0.18620689213;
            }
          }
        }
      } else {
        if ( (data[3].missing != -1) && (data[3].qvalue < 36)) {
          if ( (data[6].missing != -1) && (data[6].qvalue < 2)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 30)) {
              sum += (float)0.24000000954;
            } else {
              sum += (float)-0.030000001192;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 62)) {
              sum += (float)-0.28000000119;
            } else {
              sum += (float)0.063157901168;
            }
          }
        } else {
          if ( (data[3].missing != -1) && (data[3].qvalue < 38)) {
            sum += (float)0.28846153617;
          } else {
            if ( (data[5].missing != -1) && (data[5].qvalue < 34)) {
              sum += (float)-0.14285714924;
            } else {
              sum += (float)0.24375000596;
            }
          }
        }
      }
    } else {
      if ( (data[4].missing != -1) && (data[4].qvalue < 60)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 72)) {
          if (!(data[1].missing != -1) || (data[1].qvalue < 8)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 6)) {
              sum += (float)0.07500000298;
            } else {
              sum += (float)-0.081951223314;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 8)) {
              sum += (float)-0.16000001132;
            } else {
              sum += (float)0.13125000894;
            }
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 30)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 68)) {
              sum += (float)0.12000000477;
            } else {
              sum += (float)-0.26666668057;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 84)) {
              sum += (float)0.24062500894;
            } else {
              sum += (float)-0.057142861187;
            }
          }
        }
      } else {
        if ( (data[6].missing != -1) && (data[6].qvalue < 32)) {
          if ( (data[6].missing != -1) && (data[6].qvalue < 26)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 64)) {
              sum += (float)0.078260868788;
            } else {
              sum += (float)-0.071951217949;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 44)) {
              sum += (float)-0.28571429849;
            } else {
              sum += (float)0.15000000596;
            }
          }
        } else {
          if ( (data[5].missing != -1) && (data[5].qvalue < 46)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 0)) {
              sum += (float)-0.15000000596;
            } else {
              sum += (float)0.15000000596;
            }
          } else {
            sum += (float)0.27000001073;
          }
        }
      }
    }
  }
  if ( (data[0].missing != -1) && (data[0].qvalue < 52)) {
    if ( (data[6].missing != -1) && (data[6].qvalue < 16)) {
      if ( (data[3].missing != -1) && (data[3].qvalue < 36)) {
        if ( (data[7].missing != -1) && (data[7].qvalue < 18)) {
          if ( (data[1].missing != -1) && (data[1].qvalue < 0)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 8)) {
              sum += (float)-0.24375000596;
            } else {
              sum += (float)0.09893617034;
            }
          } else {
            if ( (data[5].missing != -1) && (data[5].qvalue < 42)) {
              sum += (float)-0.13318681717;
            } else {
              sum += (float)-0.047154475003;
            }
          }
        } else {
          if ( (data[3].missing != -1) && (data[3].qvalue < 34)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 30)) {
              sum += (float)0.26399999857;
            } else {
              sum += (float)0.13333334029;
            }
          } else {
            if ( (data[2].missing != -1) && (data[2].qvalue < 2)) {
              sum += (float)-0.25;
            } else {
              sum += (float)0.085714295506;
            }
          }
        }
      } else {
        sum += (float)0.29117646813;
      }
    } else {
      if ( (data[6].missing != -1) && (data[6].qvalue < 20)) {
        sum += (float)-0.29942747951;
      } else {
        if ( (data[0].missing != -1) && (data[0].qvalue < 28)) {
          if ( (data[6].missing != -1) && (data[6].qvalue < 22)) {
            if ( (data[7].missing != -1) && (data[7].qvalue < 4)) {
              sum += (float)-0.23571428657;
            } else {
              sum += (float)0.28571429849;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 10)) {
              sum += (float)0.15000000596;
            } else {
              sum += (float)-0.21000000834;
            }
          }
        } else {
          if ( (data[2].missing != -1) && (data[2].qvalue < 2)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 30)) {
              sum += (float)0.20000001788;
            } else {
              sum += (float)-0.13846154511;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 20)) {
              sum += (float)0.025000002235;
            } else {
              sum += (float)0.24230769277;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].missing != -1) && (data[7].qvalue < 2)) {
      if ( (data[0].missing != -1) && (data[0].qvalue < 58)) {
        if ( (data[2].missing != -1) && (data[2].qvalue < 6)) {
          sum += (float)-0.22500000894;
        } else {
          sum += (float)0.15000000596;
        }
      } else {
        if ( (data[8].missing != -1) && (data[8].qvalue < 6)) {
          sum += (float)-0.20000001788;
        } else {
          if ( (data[5].missing != -1) && (data[5].qvalue < 32)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 64)) {
              sum += (float)0.15000000596;
            } else {
              sum += (float)-0.15000000596;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 68)) {
              sum += (float)0.21176472306;
            } else {
              sum += (float)0.29350394011;
            }
          }
        }
      }
    } else {
      if ( (data[3].missing != -1) && (data[3].qvalue < 28)) {
        if ( (data[2].missing != -1) && (data[2].qvalue < 8)) {
          if ( (data[2].missing != -1) && (data[2].qvalue < 0)) {
            if ( (data[3].missing != -1) && (data[3].qvalue < 18)) {
              sum += (float)-0.025000002235;
            } else {
              sum += (float)0.24166668952;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 74)) {
              sum += (float)0.087931036949;
            } else {
              sum += (float)-0.16693548858;
            }
          }
        } else {
          if ( (data[8].missing != -1) && (data[8].qvalue < 54)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 28)) {
              sum += (float)-0.15000000596;
            } else {
              sum += (float)0.26739132404;
            }
          } else {
            sum += (float)-0.20000001788;
          }
        }
      } else {
        if ( (data[0].missing != -1) && (data[0].qvalue < 70)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 70)) {
            sum += (float)0.26250001788;
          } else {
            sum += (float)-0.27272728086;
          }
        } else {
          if ( (data[5].missing != -1) && (data[5].qvalue < 82)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 54)) {
              sum += (float)-0.15000000596;
            } else {
              sum += (float)0.24868421257;
            }
          } else {
            if ( (data[1].missing != -1) && (data[1].qvalue < 8)) {
              sum += (float)-0.1875;
            } else {
              sum += (float)0.25714287162;
            }
          }
        }
      }
    }
  }
  if ( (data[6].missing != -1) && (data[6].qvalue < 24)) {
    if ( (data[4].missing != -1) && (data[4].qvalue < 6)) {
      if ( (data[5].missing != -1) && (data[5].qvalue < 12)) {
        sum += (float)0.20000001788;
      } else {
        if ( (data[0].missing != -1) && (data[0].qvalue < 22)) {
          sum += (float)-0.29347828031;
        } else {
          if ( (data[5].missing != -1) && (data[5].qvalue < 26)) {
            if ( (data[1].missing != -1) && (data[1].qvalue < 8)) {
              sum += (float)0.20000001788;
            } else {
              sum += (float)-0.15000000596;
            }
          } else {
            sum += (float)-0.26250001788;
          }
        }
      }
    } else {
      if ( (data[6].missing != -1) && (data[6].qvalue < 22)) {
        if ( (data[8].missing != -1) && (data[8].qvalue < 2)) {
          if ( (data[3].missing != -1) && (data[3].qvalue < 8)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 10)) {
              sum += (float)-0.20000001788;
            } else {
              sum += (float)0.15000000596;
            }
          } else {
            sum += (float)-0.28636363149;
          }
        } else {
          if ( (data[8].missing != -1) && (data[8].qvalue < 58)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 14)) {
              sum += (float)0.13125000894;
            } else {
              sum += (float)-0.012800000608;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 0)) {
              sum += (float)-0;
            } else {
              sum += (float)0.28636363149;
            }
          }
        }
      } else {
        sum += (float)-0.29210525751;
      }
    }
  } else {
    if ( (data[5].missing != -1) && (data[5].qvalue < 60)) {
      if ( (data[7].missing != -1) && (data[7].qvalue < 20)) {
        if ( (data[7].missing != -1) && (data[7].qvalue < 0)) {
          if ( (data[1].missing != -1) && (data[1].qvalue < 10)) {
            sum += (float)-0.25714287162;
          } else {
            sum += (float)0.15000000596;
          }
        } else {
          if ( (data[8].missing != -1) && (data[8].qvalue < 50)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 44)) {
              sum += (float)0.22666667402;
            } else {
              sum += (float)0.053571432829;
            }
          } else {
            if ( (data[3].missing != -1) && (data[3].qvalue < 46)) {
              sum += (float)-0.24000000954;
            } else {
              sum += (float)0.15000000596;
            }
          }
        }
      } else {
        if ( (data[8].missing != -1) && (data[8].qvalue < 28)) {
          if ( (data[6].missing != -1) && (data[6].qvalue < 28)) {
            sum += (float)-0.20000001788;
          } else {
            sum += (float)0.26250001788;
          }
        } else {
          sum += (float)-0.27692309022;
        }
      }
    } else {
      if ( (data[0].missing != -1) && (data[0].qvalue < 66)) {
        if ( (data[8].missing != -1) && (data[8].qvalue < 14)) {
          if ( (data[1].missing != -1) && (data[1].qvalue < 6)) {
            sum += (float)-0.20000001788;
          } else {
            sum += (float)0.15000000596;
          }
        } else {
          if ( (data[3].missing != -1) && (data[3].qvalue < 40)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 24)) {
              sum += (float)-0.22500000894;
            } else {
              sum += (float)0.24000000954;
            }
          } else {
            sum += (float)0.29433962703;
          }
        }
      } else {
        if ( (data[8].missing != -1) && (data[8].qvalue < 42)) {
          sum += (float)0.20000001788;
        } else {
          sum += (float)-0.24000000954;
        }
      }
    }
  }
  if ( (data[5].missing != -1) && (data[5].qvalue < 72)) {
    if ( (data[0].missing != -1) && (data[0].qvalue < 52)) {
      if ( (data[5].missing != -1) && (data[5].qvalue < 22)) {
        if ( (data[5].missing != -1) && (data[5].qvalue < 16)) {
          if ( (data[4].missing != -1) && (data[4].qvalue < 20)) {
            if ( (data[8].missing != -1) && (data[8].qvalue < 26)) {
              sum += (float)-0.20000001788;
            } else {
              sum += (float)0.19615384936;
            }
          } else {
            if ( (data[6].missing != -1) && (data[6].qvalue < 12)) {
              sum += (float)-0.032142858952;
            } else {
              sum += (float)-0.28333333135;
            }
          }
        } else {
          if ( (data[1].missing != -1) && (data[1].qvalue < 6)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 8)) {
              sum += (float)-0.15000000596;
            } else {
              sum += (float)0.08749999851;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 2)) {
              sum += (float)0.25588235259;
            } else {
              sum += (float)0.070588238537;
            }
          }
        }
      } else {
        if ( (data[5].missing != -1) && (data[5].qvalue < 28)) {
          if ( (data[8].missing != -1) && (data[8].qvalue < 30)) {
            sum += (float)-0.29230770469;
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 32)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.28000000119;
            }
          }
        } else {
          if ( (data[8].missing != -1) && (data[8].qvalue < 46)) {
            if ( (data[6].missing != -1) && (data[6].qvalue < 2)) {
              sum += (float)0.083870969713;
            } else {
              sum += (float)-0.039102569222;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 56)) {
              sum += (float)-0.1685950458;
            } else {
              sum += (float)0.05000000447;
            }
          }
        }
      }
    } else {
      if ( (data[7].missing != -1) && (data[7].qvalue < 14)) {
        if ( (data[8].missing != -1) && (data[8].qvalue < 4)) {
          sum += (float)-0.20000001788;
        } else {
          if ( (data[2].missing != -1) && (data[2].qvalue < 2)) {
            if ( (data[5].missing != -1) && (data[5].qvalue < 62)) {
              sum += (float)0.20000001788;
            } else {
              sum += (float)-0.05000000447;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 44)) {
              sum += (float)0.28499999642;
            } else {
              sum += (float)0.16875000298;
            }
          }
        }
      } else {
        if ( (data[0].missing != -1) && (data[0].qvalue < 76)) {
          sum += (float)-0.25714287162;
        } else {
          sum += (float)0.15000000596;
        }
      }
    }
  } else {
    if ( (data[4].missing != -1) && (data[4].qvalue < 64)) {
      if ( (data[5].missing != -1) && (data[5].qvalue < 94)) {
        if ( (data[2].missing != -1) && (data[2].qvalue < 10)) {
          if ( (data[8].missing != -1) && (data[8].qvalue < 24)) {
            if ( (data[2].missing != -1) && (data[2].qvalue < 4)) {
              sum += (float)-0.17142859101;
            } else {
              sum += (float)0.10909091681;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 38)) {
              sum += (float)0.26037737727;
            } else {
              sum += (float)0.14676259458;
            }
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 4)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 68)) {
              sum += (float)0.12000000477;
            } else {
              sum += (float)0.26250001788;
            }
          } else {
            if ( (data[4].missing != -1) && (data[4].qvalue < 56)) {
              sum += (float)-0.14285714924;
            } else {
              sum += (float)0.22500000894;
            }
          }
        }
      } else {
        sum += (float)-0.25;
      }
    } else {
      if ( (data[0].missing != -1) && (data[0].qvalue < 74)) {
        if ( (data[0].missing != -1) && (data[0].qvalue < 0)) {
          if ( (data[6].missing != -1) && (data[6].qvalue < 10)) {
            sum += (float)0.15000000596;
          } else {
            sum += (float)-0.27500000596;
          }
        } else {
          if ( (data[4].missing != -1) && (data[4].qvalue < 66)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 62)) {
              sum += (float)-0.19285714626;
            } else {
              sum += (float)0.22500000894;
            }
          } else {
            if ( (data[0].missing != -1) && (data[0].qvalue < 56)) {
              sum += (float)0.25333335996;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[4].missing != -1) && (data[4].qvalue < 72)) {
          if ( (data[5].missing != -1) && (data[5].qvalue < 80)) {
            if ( (data[0].missing != -1) && (data[0].qvalue < 80)) {
              sum += (float)-0.20000001788;
            } else {
              sum += (float)0.26250001788;
            }
          } else {
            if ( (data[8].missing != -1) && (data[8].qvalue < 48)) {
              sum += (float)-0.25074627995;
            } else {
              sum += (float)-0.064285717905;
            }
          }
        } else {
          if ( (data[2].missing != -1) && (data[2].qvalue < 8)) {
            if ( (data[4].missing != -1) && (data[4].qvalue < 74)) {
              sum += (float)0.25714287162;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.20000001788;
          }
        }
      }
    }
  }

  sum = sum + (float)(0.5);
  if (!pred_margin) {
    return pred_transform(sum);
  } else {
    return sum;
  }
}
