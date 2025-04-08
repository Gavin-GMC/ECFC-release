
#pragma once
#include<string>

namespace FileOut
{
	class FilePNG
	{
	public:

		void plot(double* data, int number);

		void line(double* data, int number);

		void bar(double* data, int number);
	};
}

/*
#include <cairo/cairo.h>
#include <iostream>
#include <string>
#include <vector>

// ������������������ΪPNGͼƬ
void writeTableToPNG(const std::string* headers, const std::string* columnTitles, int rows, int cols, const double* data, const std::string& filename) {
    // ����ͼƬ�ߴ�
    int width = 800;
    int height = 600;
    int cellWidth = width / (cols + 1);
    int cellHeight = height / (rows + 1);

    // ����Cairo����
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* cr = cairo_create(surface);

    // ���ñ�����ɫ
    cairo_set_source_rgb(cr, 1, 1, 1); // ��ɫ
    cairo_paint(cr);

    // ���Ʊ����
    cairo_set_source_rgb(cr, 0, 0, 0); // ��ɫ
    cairo_set_line_width(cr, 1.0);

    for (int i = 0; i <= rows + 1; ++i) {
        cairo_move_to(cr, 0, i * cellHeight);
        cairo_line_to(cr, width, i * cellHeight);
    }
    for (int i = 0; i <= cols + 1; ++i) {
        cairo_move_to(cr, i * cellWidth, 0);
        cairo_line_to(cr, i * cellWidth, height);
    }
    cairo_stroke(cr);

    // ���Ʊ�ͷ
    for (int col = 0; col < cols; ++col) {
        cairo_move_to(cr, (col + 1) * cellWidth + 10, cellHeight / 2 + 20);
        cairo_show_text(cr, headers[col].c_str());
    }

    // ����ÿһ�еı��������
    for (int row = 0; row < rows; ++row) {
        cairo_move_to(cr, 10, (row + 2) * cellHeight - 10);
        cairo_show_text(cr, columnTitles[row].c_str());
        for (int col = 0; col < cols; ++col) {
            cairo_move_to(cr, (col + 1) * cellWidth + 10, (row + 2) * cellHeight - 10);
            cairo_show_text(cr, std::to_string(data[row * cols + col]).c_str());
        }
    }

    // ����ͼƬ
    cairo_surface_write_to_png(surface, filename.c_str());

    // ����
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

int main() {
    // ��ͷ
    std::string headers[] = {"Header1", "Header2", "Header3"};
    int cols = sizeof(headers) / sizeof(headers[0]);

    // ÿһ�еı���
    std::string columnTitles[] = {"Row1", "Row2", "Row3", "Row4"};
    int rows = sizeof(columnTitles) / sizeof(columnTitles[0]);

    // �������
    double data[] = {
        1.1, 2.1, 3.1,
        1.2, 2.2, 3.2,
        1.3, 2.3, 3.3,
        1.4, 2.4, 3.4
    };

    // ����ļ���
    std::string filename = "table.png";

    // ���ú���������������ΪPNGͼƬ
    writeTableToPNG(headers, columnTitles, rows, cols, data, filename);

    std::cout << "Table has been written to " << filename << std::endl;

    return 0;
}
*/