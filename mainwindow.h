#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <boost/filesystem.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    boost::filesystem::path MzFeaturesOutFolder;
    boost::filesystem::path PredictorOutputFolderRoot;
    boost::filesystem::path PredictorOutputFolder;
    boost::filesystem::path OutputFolder;

    std::vector<std::string> PredictorOutputFileNamesVector;

    std::vector<int> PredictedClassIntVector;
    std::vector<std::string> SubfoldersNamesVector;
    std::vector<std::string> PredictedClassStrVector;
    std::vector<std::string> TrueClassStrVector;

    int samplesCount;
    int ErrorCount;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool ReadPredictorSubfolders();
    void OpenPredictorOutputFotder();
    std::string ShowResult(std::string FileNameStr, bool autoClear = true);
    //string EvaluatePrediction(int fileNrToProcess,std::vector<std::string> PredictorOutputFileNamesVector);

private slots:
    void on_pushButtonOpenPredictorOutputFolder_clicked();

    void on_pushButtonOpenOutFolder_clicked();

    void on_listWidgetPredictorOutFiles_currentTextChanged(const QString &currentText);

    void on_pushButtonProcessAllFiles_clicked();

    void on_pushButtonReload_clicked();



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
