#pragma once
#include <string>
#include <fstream>
#include <vector>

namespace FileOut
{
    // ������������������ΪCSV�ļ�
    bool writeTableToCSV(const std::vector<std::string>& headers,
        const std::vector<std::string>& subheaders,
        const std::vector<std::string>& rowtitles,
        const std::vector<std::vector<std::string>>& tabledata,
        const std::string& filepath)
    {
        // ���ļ�
        std::ofstream outFile(filepath);
        if (!outFile.is_open()) {
            return false;
        }

        int colomns;
        if (subheaders.size() == 0)
            colomns = headers.size();
        else
            colomns = headers.size() * subheaders.size();

        // ��������
        int after_t;

        // д���ͷ
        after_t = (colomns - headers.size()) / headers.size();
        outFile << ",";
        for (int col = 0; col < headers.size(); col++)
        {
            outFile << headers[col];
            for (int i = 0; i < after_t; i++)
                outFile << ",";
            if (col < headers.size() - 1) {
                outFile << ","; // ʹ�ö��ŷָ�
            }
        }
        outFile << "\n";

        // д���ӱ���
        if (subheaders.size() != 0)
        {
            outFile << ",";
            for (int col = 0; col < headers.size(); col++)
            {
                for (int subcol = 0; subcol < subheaders.size(); subcol++)
                {
                    outFile << subheaders[subcol];
                    if (subcol < subheaders.size() - 1) {
                        outFile << ","; // ʹ���Ʊ���ָ�
                    }
                }
                if (col < headers.size() - 1) {
                    outFile << ","; // ʹ���Ʊ���ָ�
                }
            }
            outFile << "\n";
        }

        // д������
        for (int row = 0; row < rowtitles.size(); ++row)
        {
            outFile << rowtitles[row] << ","; // ÿ�еı���

            if (tabledata[row].size() == 0) // ����ȱʧ
            {
                outFile << "\n";
                continue;
            }

            // ������
            after_t = (colomns - tabledata[row].size()) / tabledata[row].size();
            for (int col = 0; col < tabledata[row].size(); ++col) {
                outFile << tabledata[row][col];
                for (int i = 0; i < after_t; i++)
                    outFile << ",";
                if (col < tabledata[row].size() - 1) {
                    outFile << ","; // ʹ���Ʊ���ָ�
                }
            }
            outFile << "\n";
        }

        // �ر��ļ�
        outFile.close();

        return true;
    }

}
/*
#include <iostream>
#include <string>
#include <vector>
#include <xlsxwriter.h>

// ��������������������Excel�ļ�
void writeTableToExcel(const std::string* headers, const std::string* columnTitles, int rows, int cols, const double* data, const std::string& filename) {
    // ����һ���µ�Excel�ļ�
    lxw_workbook* workbook = workbook_new(filename.c_str());
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, NULL);

    // д���ͷ
    for (int col = 0; col < cols; ++col) {
        worksheet_write_string(worksheet, 0, col, headers[col].c_str(), NULL);
    }

    // д��ÿһ�еı���
    for (int row = 0; row < rows; ++row) {
        worksheet_write_string(worksheet, row + 1, 0, columnTitles[row].c_str(), NULL);
    }

    // д������
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            worksheet_write_number(worksheet, row + 1, col + 1, data[row * cols + col], NULL);
        }
    }

    // �رչ�����
    workbook_close(workbook);
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
    std::string filename = "output.xlsx";

    // ���ú�����������������Excel�ļ�
    writeTableToExcel(headers, columnTitles, rows, cols, data, filename);

    std::cout << "Table has been written to " << filename << std::endl;

    return 0;
}
*/