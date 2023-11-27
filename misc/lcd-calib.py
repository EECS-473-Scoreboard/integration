import numpy as np

# [x, y, raw_x, raw_y]
data = [
    [80, 40, 64304, 61479],
    [400, 40, 61391, 57760],
    [720, 40, 56247, 55313],
    [80, 200, 63008, 57471],
    [400, 200, 57391, 48938],
    [720, 200, 50316, 45748],
    [80, 360, 62262, 52872],
    [400, 360, 54694, 39386],
    [720, 360, 43174, 32057],
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