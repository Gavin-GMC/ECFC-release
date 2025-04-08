#pragma once
#include<string>
#include<fstream>
#include<vector>

namespace FileOut
{
    // ������������������ΪLaTeX��ʽ���ַ���
    // ������������������Ϊ������LaTeX�ļ�
    bool writeTableToLaTeX(const std::vector<std::string>& headers,
        const std::vector<std::string>& subheaders,
        const std::vector<std::string>& rowtitles,
        const std::vector<std::vector<std::string>>& tabledata,
        const std::string& filepath,
        const std::vector<std::vector<int>>& bolddata,
        bool ThreeLine = true)
    {
        // ���ļ�
        std::ofstream outFile(filepath);
        if (!outFile.is_open()) {
            return false;
        }

        int subcolomn_number;
        if (subheaders.size() == 0)
            subcolomn_number = 1;
        else
            subcolomn_number = subheaders.size();
        int max_colomns = subcolomn_number * headers.size();

        // д��LaTeX�ĵ�ͷ����Ϣ
        outFile << "\\documentclass{article}\n";
        outFile << "\\usepackage[utf8]{inputenc}\n";
        outFile << "\\usepackage{geometry}\n";
        outFile << "\\usepackage{multirow}\n";
        outFile << "\\geometry{a4paper, margin=1in}\n";
        outFile << "\\begin{document}\n\n";

        // д����ʼ���
        outFile << "\\begin{table}[ht]\n";
        outFile << "\\centering\n";
        if (ThreeLine) // ���߱�
        {
            std::string format = "|c|";
            std::string colomns_format = std::string(subcolomn_number, 'c') + "|";
            for (int i = 0; i < headers.size(); i++)
            {
                format += colomns_format;
            }
            outFile << "\\begin{tabular}{" << format << "}\n";
        }
        else // ȫ��
        {
            std::string format = "|c|";
            for (int i = 0; i < max_colomns; i++)
            {
                format += "c|";
            }
            outFile << "\\begin{tabular}{" << format << "}\n";
        }
        outFile << "\\hline\n";

        // д���ͷ
        if (subheaders.size() == 0) // ���ӱ���
        {
            // д�����
            outFile << " & ";
            for (int col = 0; col < headers.size(); col++) {
                outFile << headers[col];
                if (col < headers.size() - 1) {
                    outFile << " & ";
                }
            }
            outFile << " \\\\ \\hline\n";
        }
        else
        {
            // д�����
            outFile << "\\multirow{2}{*}{ } & ";
            for (int col = 0; col < headers.size(); col++) {
                outFile << "\\multicolumn{" << subheaders.size() << "}{c|}{" << headers[col] << "}";
                if (col < headers.size() - 1) {
                    outFile << " & ";
                }
            }
            outFile << " \\\\ \\cline{2-" + std::to_string(1 + max_colomns) + "}\n";

            // д���ӱ���
            outFile << " & ";
            for (int col = 0; col < headers.size(); col++) {
                for (int subcol = 0; subcol < subheaders.size(); subcol++) {
                    outFile << subheaders[subcol];
                    if (subcol < subheaders.size() - 1) {
                        outFile << " & ";
                    }
                }
                if (col < headers.size() - 1) {
                    outFile << " & ";
                }
            }
            outFile << " \\\\ \\hline\n";
        }

        // д������
        for (int row = 0; row < rowtitles.size(); ++row)
        {
            bool* bold = new bool[tabledata[row].size()];
            for (int i = 0; i < tabledata[row].size(); i++)
                bold[i] = false;
            if (row < bolddata.size())
                for (int i = 0; i < bolddata[row].size(); i++)
                    bold[bolddata[row][i]] = true;

            outFile << rowtitles[row] << " & ";

            if (tabledata[row].size() != max_colomns)
            {
                int merges = max_colomns / tabledata[row].size();
                for (int col = 0; col < tabledata[row].size(); ++col) {
                    outFile << "\\multicolumn{" + std::to_string(merges) + "}{c|}{" << tabledata[row][col] << "}";
                    if (col < tabledata[row].size() - 1) {
                        outFile << " & ";
                    }
                }
            }
            else
            {
                for (int col = 0; col < tabledata[row].size(); ++col) {
                    outFile << (bold[col] ? "\\textbf{" + tabledata[row][col] + "}" : tabledata[row][col]);
                    ;
                    if (col < tabledata[row].size() - 1) {
                        outFile << " & ";
                    }
                }
            }

            if (row == rowtitles.size() - 1 || !ThreeLine || tabledata[row].size() != tabledata[row + 1].size())
                outFile << " \\\\ \\hline\n";
            else
                outFile << " \\\\ \n";

            delete[] bold;
        }

        // д����������
        outFile << "\\end{tabular}\n";
        outFile << "\\caption{Table}\n";
        outFile << "\\label{table:example}\n";
        outFile << "\\end{table}\n\n";

        // д��LaTeX�ĵ�β����Ϣ
        outFile << "\\end{document}\n";

        // �ر��ļ�
        outFile.close();

        return true;
    }
}