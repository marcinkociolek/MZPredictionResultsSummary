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
    boost::filesystem::path PredictorOutputFolder;
    boost::filesystem::path OutputFolder;

    std::vector<std::string> PredictorOutputFileNamesVector;

    std::vector<int> PredictedClassIntVector;
    std::vector<std::string> PredictedClassStrVector;
    std::vector<std::string> TrueClassStrVector;

    int samplesCount;
    int ErrorCount;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void OpenPredictorOutputFotder();
    void OutputFotder();
    void ShowResult();
    //string EvaluatePrediction(int fileNrToProcess,std::vector<std::string> PredictorOutputFileNamesVector);

private slots:
    void on_pushButtonOpenPredictorOutputFolder_clicked();

    void on_pushButtonOpenOutFolder_clicked();

    void on_spinBoxPredictedFileToShow_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
