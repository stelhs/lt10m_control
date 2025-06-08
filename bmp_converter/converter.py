import struct
from pathlib import Path
import os


def read_bmp(file_path):
    with open(file_path, 'rb') as f:
        # Читаем заголовок BMP файла
        header = f.read(54)  # Заголовок BMP занимает 54 байта

        # Извлекаем ширину и высоту из заголовка
        width, height = struct.unpack('<ii', header[18:26])

        # Проверяем формат файла (должен быть 24-битный BMP)
        bpp = struct.unpack('<H', header[28:30])[0]  # Bits per pixel
        if bpp != 24:
            raise ValueError("Этот код поддерживает только BMP файлы с 24 битами на пиксель (RGB24).")

        # Вычисляем количество байт на строку изображения (учитываем выравнивание до 4 байт)
        row_padded = (width * 3 + 3) & ~3

        # Читаем пиксельные данные
        pixel_data = []
        for y in range(height):
            row = f.read(row_padded)  # Читаем строку с учетом выравнивания
            row_pixels = []
            for x in range(width):
                # Каждый пиксель состоит из трех байтов: B, G, R
                b, g, r = row[x*3:x*3+3]
                row_pixels.append((r, g, b))  # Добавляем в массив в порядке RGB

            # Добавляем строку в начало списка (чтобы перевернуть порядок строк)
            pixel_data.insert(0, row_pixels)

        # Объединяем все строки в один список
        pixel_data = [pixel for row in pixel_data for pixel in row]

        return width, height, pixel_data


def convert_file(img_name, f_name):
    width, height, pixels = read_bmp(f_name)
    print("%s size: %sx%s, %d bytes" % (img_name, width, height,
                                        width * height * 3))
    text = 'struct img *img_%s(void)\n' % img_name
    text += '{\n'
    text += '    struct img *img = img_alloc("img", %d, %d);\n' % (width, height)
    text += '    u8 *p = img->buf->d;\n    '
    cols = 0
    i = 0
    for pix in pixels:
        if pix[0] or pix[1] or pix[2]:
            if pix[0]:
                text += 'p[%d] = 0x%02X; /* red */ ' % (i, pix[0])
            i += 1
            if pix[1]:
                text += 'p[%d] = 0x%02X; /* green */ ' % (i, pix[1])
            i += 1
            if pix[2]:
                text += 'p[%d] = 0x%02X; /* blue */ ' % (i, pix[2])
            i += 1
            text += "\n    "
        else:
            i += 3

    text += "\n    return img;\n"
    text += "}\n\n"
    return text



img_files = os.listdir('images/')

text_c = '#include "images.h"\n'
text_c += '#include "stm32_lib/buf.h"\n\n'


text_h = '#ifndef SRC_IMAGES_H_\n'
text_h += '#define SRC_IMAGES_H_\n\n'
text_h += '#include "disp_mipi_dcs.h"\n\n'


for f_name in img_files:
    img_name = Path(f_name).resolve().stem
    text_c += convert_file(img_name, '%s%s' % ('images/', f_name))
    text_h += 'struct img *img_%s(void);\n' % img_name

text_h += '\n#endif /* SRC_IMAGES_H_ */\n'

with open("../Core/Src/machine/images.c", "w") as f:
    f.write(text_c)

with open("../Core/Src/machine/images.h", "w") as f:
    f.write(text_h)

