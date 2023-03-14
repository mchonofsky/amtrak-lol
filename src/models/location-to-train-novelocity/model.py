import onnxruntime as rt
from sklearn.datasets import load_iris, load_diabetes, make_classification
from sklearn.model_selection import train_test_split
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from xgboost import XGBClassifier, XGBRegressor, DMatrix, train as train_xgb
from skl2onnx.common.data_types import FloatTensorType
from skl2onnx import convert_sklearn, to_onnx, update_registered_converter
from skl2onnx.common.shape_calculator import (
    calculate_linear_classifier_output_shapes,
    calculate_linear_regressor_output_shapes)
from onnxmltools.convert.xgboost.operator_converters.XGBoost import (
    convert_xgboost)
from onnxmltools.convert import convert_xgboost as convert_xgboost_booster
from xgboost import XGBClassifier
from sklearn.model_selection import cross_val_score
import pandas as pd, numpy as np, xgboost as xgb
from sklearn.model_selection import StratifiedKFold

X = pd.read_csv('training_data.csv').to_numpy()[:,1:-2]
y = pd.read_csv('training_data.csv').to_numpy()[:,0]
strat_k_fold = StratifiedKFold(n_splits=5, shuffle=True)
xc = XGBClassifier(max_depth=6, eta=0.3, tree_method='hist', objective='binary:hinge', eval_metric=['error', 'logloss'], subsample=0.5)

scores = cross_val_score(xc, X, y, cv=strat_k_fold, scoring="accuracy")
xc.fit(X,y)

pipe = Pipeline([('xgb', xc)])
update_registered_converter(
    XGBClassifier, 'XGBoostXGBClassifier',
    calculate_linear_classifier_output_shapes, convert_xgboost,
    options={'nocl': [True, False], 'zipmap': [True, False, 'columns']})
model_onnx = convert_sklearn(
    pipe, 'pipeline_xgboost',
    [('input', FloatTensorType([None, X.shape[1]]))],
    target_opset=12, options={'zipmap': False}
    )
with open("pipeline_xgboost.onnx", "wb") as f:
    f.write(model_onnx.SerializeToString())
