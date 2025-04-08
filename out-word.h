
#pragma once
#include<string>

namespace FileOut
{
	class FileWord
	{
	public:

		void table(std::string* data, int height, int length);

	};
}

/*
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <docx/document.h>
#include <docx/paragraph.h>
#include <docx/table.h>
#include <docx/table_row.h>
#include <docx/table_cell.h>
#include <docx/run.h>
#include <docx/text.h>

// ��������������������Word�ļ�
void writeTableToWord(const std::string* headers, const std::string* columnTitles, int rows, int cols, const double* data, const std::string& filename) {
    // ����һ���µ�Word�ĵ�
    docx::Document doc;

    // �������
    docx::Table table(rows + 1, cols + 1);

    // д���ͷ
    docx::TableRow& headerRow = table.row(0);
    headerRow.cell(0).addParagraph().addRun().addText(" ");
    for (int col = 0; col < cols; ++col) {
        headerRow.cell(col + 1).addParagraph().addRun().addText(headers[col]);
    }

    // д��ÿһ�еı��������
    for (int row = 0; row < rows; ++row) {
        docx::TableRow& dataRow = table.row(row + 1);
        dataRow.cell(0).addParagraph().addRun().addText(columnTitles[row]);
        for (int col = 0; col < cols; ++col) {
            dataRow.cell(col + 1).addParagraph().addRun().addText(std::to_string(data[row * cols + col]));
        }
    }

    // �������ӵ��ĵ���
    doc.addTable(table);

    // �����ĵ�
    doc.save(filename);
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
    std::string filename = "output.docx";

    // ���ú�����������������Word�ļ�
    writeTableToWord(headers, columnTitles, rows, cols, data, filename);

    std::cout << "Table has been written to " << filename << std::endl;

    return 0;
}
*/