#include "widget.h"
#include "nlohmann_json.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

void Widget::exportGraph()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save As"));

    if (!filePath.isEmpty())
    {
        QFile file(filePath);

        if (file.open(QFile::WriteOnly))
        {
            QTextStream fileStream(&file);
            fileStream << m_nodes.size() << '\n';

            for (int i = 0; i < m_edgeMatrix.size(); ++i)
            {
                for (int j = 0; j < m_edgeMatrix.size(); ++j)
                {
                    if (m_edgeMatrix[i][j])
                        fileStream << i << ' '<< j << '\n';
                }
            }
        }
        else
        {
            QMessageBox::warning(this, tr("Error"), "Failed to save file: " + filePath);
        }
    }
}

void Widget::saveAsJson()
{
    nlohmann::json document;
    auto& nodePositions = document["nodes"];

    for (int i = 0; i != m_nodes.size(); ++i)
    {
        nodePositions.push_back({
            m_nodes[i].circle().x / size().width(),
            m_nodes[i].circle().y / size().height()
        });
    }

    auto& edges = document["edges"];

    for (int i = 0; i != m_edgeMatrix.size(); ++i)
    {
        for (int j = 0; j != m_edgeMatrix.size(); ++j)
        {
            if (m_edgeMatrix[i][j])
                edges.push_back({ i, j });
        }
    }

    QString filePath = QFileDialog::getSaveFileName(this, tr("Save As"));

    if (!filePath.isEmpty())
    {
        QFile file(filePath);

        if (file.open(QFile::WriteOnly))
        {
            QTextStream textStream(&file);
            textStream << document.dump(4).c_str();

            QMessageBox::information(this, tr("Success!"), "File saved successfully!");
        }
        else
        {
            QMessageBox::warning(this, tr("Error"), "Failed to save file: " + filePath);
        }
    }
}

void Widget::loadFromJson()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"));

    if (!filePath.isEmpty())
    {
        QFile file(filePath);

        if (file.open(QFile::ReadOnly))
        {
            QTextStream textStream(&file);

            try
            {
                nlohmann::json document = nlohmann::json::parse(textStream.readAll().toStdString());

                clearNodes();

                for (const auto& position : document["nodes"])
                {
                    m_nodes.push_back(Node(
                        position[0].get<float>() * size().width(),
                        position[1].get<float>() * size().height()
                    ));
                }

                m_edgeMatrix.resize(m_nodes.size());

                for (auto& line : m_edgeMatrix)
                    line.resize(m_nodes.size());

                int i = 0, j = 0;

                for (const auto& edge : document["edges"])
                {
                    i = edge[0].get<int>();
                    j = edge[1].get<int>();

                    if (i >= 0 && i < m_nodes.size() && j >= 0 && j < m_nodes.size())
                    {
                        m_edgeMatrix[i][j] = true;
                    }
                }
            }
            catch (const std::exception& error)
            {
                clearNodes();
                QMessageBox::warning(this, tr("Parse Error"), error.what());
            }
        }
        else
        {
            clearNodes();
            QMessageBox::warning(this, tr("Error"), "Failed to open file: " + filePath);
        }
    }

    repaint();
}
