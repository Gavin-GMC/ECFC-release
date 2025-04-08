#pragma once
#include<string>
#include<fstream>
#include<vector>

namespace FileOut
{
    // ������������������ΪTXT�ļ���ʹ���Ʊ����Ϊ�ָ���
    bool writeTableToTxt(const std::vector<std::string>& headers,
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
        outFile << "\t";
        for (int col = 0; col < headers.size(); col++)
        {
            outFile << headers[col];
            for (int i = 0; i < after_t; i++)
                outFile << "\t";
            if (col < headers.size() - 1) {
                outFile << "\t"; // ʹ���Ʊ���ָ�
            }
        }
        outFile << "\n";

        // д���ӱ���
        if (subheaders.size() != 0)
        {
            outFile << "\t";
            for (int col = 0; col < headers.size(); col++)
            {
                for (int subcol = 0; subcol < subheaders.size(); subcol++)
                {
                    outFile << subheaders[subcol];
                    if (subcol < subheaders.size() - 1) {
                        outFile << "\t"; // ʹ���Ʊ���ָ�
                    }
                }
                if (col < headers.size() - 1) {
                    outFile << "\t"; // ʹ���Ʊ���ָ�
                }
            }
            outFile << "\n";
        }

        // д������
        for (int row = 0; row < rowtitles.size(); ++row)
        {
            outFile << rowtitles[row] << "\t"; // ÿ�еı���

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
                    outFile << "\t";
                if (col < tabledata[row].size() - 1) {
                    outFile << "\t"; // ʹ���Ʊ���ָ�
                }
            }
            outFile << "\n";
        }

        // �ر��ļ�
        outFile.close();

        return true;
    }
}