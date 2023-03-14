static const int th_len[] = {
  6, 0, 0, 2, 3, 9, 1, 4, 5, 
};
static const int th_begin[] = {
  0, 6, 6, 6, 8, 11, 20, 21, 25, 
};
static const float threshold[] = {
  0.0086992048, 3.6708183, 4.6953373, 5.0080061, 6.8392887, 40.178635, 
  37.338539, 40.772461, -87.639046, -77.297379, -75.182091, 0.0022284458, 
  0.035416063, 0.11374504, 0.11528733, 1.5415967, 1.7220037, 10.139078, 
  12.397341, 22.548248, 42.648033, -90.205933, -87.639252, -73.994019, 
  -71.059738, 9.5, 36.5, 96, 180.5, 300.5, 
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
  if (offset == 30 || val < array[0]) {
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
  return "sigmoid";
}

float get_sigmoid_alpha(void) {
  return 1.0;
}

float get_ratio_c(void) {
  return 1.0;
}

float get_global_bias(void) {
  return -0.0;
}

const char* get_threshold_type(void) {
  return "float32";
}

const char* get_leaf_output_type(void) {
  return "float32";
}


static inline float pred_transform(float margin) {
  const float alpha = (float)1.0;
  return (float)(1) / ((float)(1) + expf(-alpha * margin));
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
  if (!(data[0].missing != -1) || (data[0].qvalue < 6)) {
    if (!(data[5].missing != -1) || (data[5].qvalue < 14)) {
      sum += (float)-1.5263999701;
    } else {
      sum += (float)1.2540540695;
    }
  } else {
    if (!(data[8].missing != -1) || (data[8].qvalue < 4)) {
      sum += (float)-1.2195122242;
    } else {
      sum += (float)1.6503732204;
    }
  }
  if (!(data[5].missing != -1) || (data[5].qvalue < 6)) {
    if (!(data[0].missing != -1) || (data[0].qvalue < 4)) {
      sum += (float)4.8257880211;
    } else {
      sum += (float)1.1642030478;
    }
  } else {
    if (!(data[5].missing != -1) || (data[5].qvalue < 16)) {
      sum += (float)-0.84058487415;
    } else {
      sum += (float)0.61875665188;
    }
  }
  if (!(data[4].missing != -1) || (data[4].qvalue < 2)) {
    if (!(data[8].missing != -1) || (data[8].qvalue < 8)) {
      sum += (float)0.28232598305;
    } else {
      sum += (float)1.0819193125;
    }
  } else {
    if (!(data[3].missing != -1) || (data[3].qvalue < 2)) {
      sum += (float)-1.1726679802;
    } else {
      sum += (float)0.14354352653;
    }
  }
  if (!(data[6].missing != -1) || (data[6].qvalue < 0)) {
    if (!(data[0].missing != -1) || (data[0].qvalue < 0)) {
      sum += (float)-0.59625172615;
    } else {
      sum += (float)0.19522581995;
    }
  } else {
    if (!(data[0].missing != -1) || (data[0].qvalue < 2)) {
      sum += (float)2.7735309601;
    } else {
      sum += (float)0.66191095114;
    }
  }
  if (!(data[4].missing != -1) || (data[4].qvalue < 4)) {
    if (!(data[0].missing != -1) || (data[0].qvalue < 10)) {
      sum += (float)-0.026513900608;
    } else {
      sum += (float)0.73855090141;
    }
  } else {
    if (!(data[7].missing != -1) || (data[7].qvalue < 4)) {
      sum += (float)-0.94008713961;
    } else {
      sum += (float)0.010022869334;
    }
  }
  if (!(data[8].missing != -1) || (data[8].qvalue < 2)) {
    if (!(data[8].missing != -1) || (data[8].qvalue < 0)) {
      sum += (float)-1.8581087589;
    } else {
      sum += (float)-0.92151802778;
    }
  } else {
    if (!(data[5].missing != -1) || (data[5].qvalue < 8)) {
      sum += (float)0.77542877197;
    } else {
      sum += (float)-0.032470371574;
    }
  }
  if (!(data[5].missing != -1) || (data[5].qvalue < 12)) {
    if (!(data[5].missing != -1) || (data[5].qvalue < 0)) {
      sum += (float)1.0300406218;
    } else {
      sum += (float)-0.44530820847;
    }
  } else {
    if (!(data[3].missing != -1) || (data[3].qvalue < 0)) {
      sum += (float)0.80249643326;
    } else {
      sum += (float)0.12510931492;
    }
  }
  if (!(data[5].missing != -1) || (data[5].qvalue < 4)) {
    if (!(data[0].missing != -1) || (data[0].qvalue < 8)) {
      sum += (float)-2.6946623325;
    } else {
      sum += (float)0.88723903894;
    }
  } else {
    if (!(data[5].missing != -1) || (data[5].qvalue < 10)) {
      sum += (float)0.7711648345;
    } else {
      sum += (float)-0.024915304035;
    }
  }
  if (!(data[5].missing != -1) || (data[5].qvalue < 2)) {
    if (!(data[8].missing != -1) || (data[8].qvalue < 6)) {
      sum += (float)0.20959180593;
    } else {
      sum += (float)1.2126435041;
    }
  } else {
    if (!(data[7].missing != -1) || (data[7].qvalue < 6)) {
      sum += (float)0.016546141356;
    } else {
      sum += (float)-0.85021406412;
    }
  }
  if (!(data[7].missing != -1) || (data[7].qvalue < 2)) {
    if (!(data[7].missing != -1) || (data[7].qvalue < 0)) {
      sum += (float)0.065955966711;
    } else {
      sum += (float)-0.92961627245;
    }
  } else {
    if (!(data[4].missing != -1) || (data[4].qvalue < 0)) {
      sum += (float)3.8811311722;
    } else {
      sum += (float)0.083774663508;
    }
  }

  sum = sum + (float)(-0);
  if (!pred_margin) {
    return pred_transform(sum);
  } else {
    return sum;
  }
}
