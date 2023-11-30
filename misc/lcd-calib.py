import numpy as np

# [x, y, raw_x, raw_y]
data = [
    [80, 40, 64280, 61446],
    [240, 40, 62695, 58867],
    [400, 40, 61104, 57429],
    [560, 40, 58910, 55936],
    [720, 40, 55551, 55149],

    [80, 120, 63418, 59104],
    [240, 120, 60896, 54663],
    [400, 120, 58384, 52097],
    [560, 120, 55242, 50583],
    [720, 120, 51728, 50224],

    [80, 200, 62783, 56672],
    [240, 200, 59708, 50987],
    [400, 200, 56367, 47132],
    [560, 200, 52319, 44817],
    [720, 200, 48378, 44375],

    [80, 280, 62397, 54422],
    [240, 280, 58532, 46433],
    [400, 280, 55076, 42444],
    [560, 280, 49866, 38870],
    [720, 280, 45008, 38514],

    [80, 360, 61937, 51372],
    [240, 360, 58079, 42888],
    [400, 360, 54206, 37758],
    [560, 360, 49266, 33819],
    [720, 360, 42578, 31841],
]
ADC_MAX = 65536
ORDER = 2

a = []
b = []
for i in range(len(data)):
    x = data[i][2] / ADC_MAX
    y = data[i][3] / ADC_MAX
    actual_x = data[i][0]
    actual_y = data[i][1]

    # [1, 0, x, 0, y, 0, x^2, 0, y^2, 0, ...] = actual_x
    # [0, 1, 0, x, 0, y, 0, x^2, 0, y^2, ...] = actual_y
    x_equation = [1, 0]
    y_equation = [0, 1]
    for j in range(1, ORDER + 1):
        x_equation.extend([x ** j, 0, y ** j, 0])
        y_equation.extend([0, x ** j, 0, y ** j])
    a.append(x_equation)
    b.append(actual_x)
    a.append(y_equation)
    b.append(actual_y)

coefficients = np.linalg.lstsq(a, b, rcond = None)[0]

x_coefficients = []
y_coefficients = []
for i in range(2 * ORDER + 1):
    x_coefficients.append(coefficients[2*i])
    y_coefficients.append(coefficients[2*i + 1])

print("double LCD_CALIB_X = " + str(x_coefficients) + ';')
print("double LCD_CALIB_Y = " + str(y_coefficients) + ';')