import sys
import time
from pathlib import Path
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

project_path = Path.cwd().parent

sys.path.append(f'{project_path}/common_data/common_lib')
import my_module as mm
import cosmo_lib as csmlib

sys.path.append(f'{project_path}/common_data/common_config')
import ISTF_fid_params
import mpl_cfg

import cosmolike_utils as cu

matplotlib.use('Qt5Agg')
plt.rcParams.update(mpl_cfg.mpl_rcParams_dict)
start_time = time.perf_counter()


save_path = 'path-to-save-nz.nz'
z_min = 0
z_max = 5.3423
z_steps = 10000
z_mean = [0.65, 0.95, 1.25, 1.55, 1.85]
z_std = 0.3

niz_test = cu.create_nz(save_path, z_min=z_min, z_max=z_max, z_steps=z_steps, z_mean=z_mean, z_std=z_std)

