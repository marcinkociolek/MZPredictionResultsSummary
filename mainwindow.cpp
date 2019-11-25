#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <string>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

using namespace boost;
using namespace std;
using namespace boost::filesystem;
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          My functions outside the Mainwindow class
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
void ReadFolder(path FileFolder, std::vector<std::string> *FileNamesVector, string FilePaternStr = ".+")
{
    FileNamesVector->clear();
    if (!exists(FileFolder))
    {
        return;
    }
    if (!is_directory(FileFolder))
    {
        return;
    }
    regex FilePattern(FilePaternStr);
    for (directory_entry& FileToProcess : directory_iterator(FileFolder))
    {

        if (!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern ))
            continue;
        path PathLocal = FileToProcess.path();

        FileNamesVector->push_back(PathLocal.filename().string());
    }
    std::sort(FileNamesVector->begin(),FileNamesVector->end());
}
//------------------------------------------------------------------------------------------------------------------------------
string StringVectorToString(std::vector<std::string> FileNamesVector)
{
    string OutStr = "";
    if(FileNamesVector.empty())
        return OutStr;
    for(vector<string>::iterator iFileNamesVector = FileNamesVector.begin(); iFileNamesVector != FileNamesVector.end(); iFileNamesVector++)
    {
        OutStr += *iFileNamesVector + "\n";
    }

    return OutStr;
}
//------------------------------------------------------------------------------------------------------------------------------
QList<QString> StringVectorQStringQList(std::vector<std::string> FileNamesVector)
{
    QList<QString> OutQList;
    if(FileNamesVector.empty())
        return OutQList;
    for(vector<string>::iterator iFileNamesVector = FileNamesVector.begin(); iFileNamesVector != FileNamesVector.end(); iFileNamesVector++)
    {
        OutQList.append(QString::fromStdString(*iFileNamesVector));
    }

    return OutQList;
}
//------------------------------------------------------------------------------------------------------------------------------
string EvaluatePrediction(path PredictedFileToProcess, bool verboseMode = 0)
{
    string OutStr = "";
    if(verboseMode)
    {
        OutStr += "\n";
        OutStr += "File\t";
        OutStr += "#Samples\t";
        OutStr += "#Error\t";
        OutStr += "Error[%]\n";
    }


    string OutStr1 = "";
    if(!exists(PredictedFileToProcess))
        return "file" + PredictedFileToProcess.string() + "does not exists";

    //std::vector<int> PredictedIntVector;
    //std::vector<std::string> PredictedStdVector;
    //std::vector<std::string> TrueStdVector;

    std::ifstream inFile(PredictedFileToProcess.string());
    if (!inFile.is_open())
    {
        return "file" + PredictedFileToProcess.string() + "unable to open";
    }
    // ------------------read params from file-----------------------------

    string Line;


    int samplesCount = 0;
    int errorCount = 0;
    size_t position;
    while(inFile.good())
    {
        samplesCount++;

        getline(inFile,Line);

        if(Line == "")
            break;

        position = Line.find(" ");
        string predictedInt = Line.substr(0,position);
        Line = Line.substr(position + 1);

        position = Line.find(" : ");
        string predictedStr = Line.substr(0,position);
        Line = Line.substr(position + 3);

        position = Line.find(" ");
        string trueStr = Line.substr(0,position);


        string Result;
        if(predictedStr == trueStr)
        {
            Result = " OK";
        }
        else
        {
            Result = " False";
            errorCount++;
        }

        OutStr1 += to_string(samplesCount)
                  + "\t" + predictedInt
                  + "\t" + predictedStr
                  + "\t" + trueStr
                  + "\t" + Result
                  + "\n";

    }
    OutStr += PredictedFileToProcess.stem().string() + "\t";
    OutStr += to_string(samplesCount) + "\t";
    OutStr += to_string(errorCount) + "\t";
    if(samplesCount)
    {
       double errorPerc = (double)errorCount/(double)samplesCount * 100.0;
       OutStr += to_string(errorPerc) + "\n";
    }
    else
        OutStr += "-1\n";

    if(verboseMode)
    {
        OutStr += "\n";
        OutStr += "-----------------------\n";
        OutStr += "     Details\n";
        OutStr += "-----------------------\n";

        OutStr += OutStr1;
    }


    inFile.close();
    return OutStr;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          My functions in the Mainwindow class
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::OpenPredictorOutputFotder()
{
    if (!exists(PredictorOutputFolder))
    {
        ui->textEditOut->append(QString::fromStdString("Predictor Output folder : " + PredictorOutputFolder.string()+ " not exists "));
    }
    if (!is_directory(PredictorOutputFolder))
    {
        ui->textEditOut->append(QString::fromStdString( "Predicto rOutput folder : " + PredictorOutputFolder.string()+ " This is not a directory path "));
    }
    ui->lineEditPredictorOutputFolder->setText(QString::fromStdString(PredictorOutputFolder.string()));
    ReadFolder(PredictorOutputFolder, &PredictorOutputFileNamesVector);


    ui->listWidgetPredictorOutFiles->clear();
    ui->listWidgetPredictorOutFiles->addItems(StringVectorQStringQList(PredictorOutputFileNamesVector));
    if(ui->listWidgetPredictorOutFiles->count())
        ui->listWidgetPredictorOutFiles->setCurrentRow(0);

}
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::OutputFotder()
{
    if (!exists(OutputFolder))
    {
        ui->textEditOut->append(QString::fromStdString("Output folder : " + OutputFolder.string()+ " not exists "));
    }
    if (!is_directory(OutputFolder))
    {
        ui->textEditOut->append(QString::fromStdString( "Output folder : " + OutputFolder.string()+ " This is not a directory path "));
    }
    ui->lineEditOutFolder->setText(QString::fromStdString(OutputFolder.string()));

}
//------------------------------------------------------------------------------------------------------------------------------
string MainWindow::ShowResult(string FileNameStr, bool autoClear)
{
    path PredictorOutputFile = PredictorOutputFolder;
    PredictorOutputFile.append(FileNameStr);
    string OutText =  EvaluatePrediction(PredictorOutputFile, ui->checkBoxShowPredictionDetails->checkState());
    if(autoClear)
        ui->textEditOutFileContent->clear();
    ui->textEditOutFileContent->append(QString::fromStdString(OutText));
    return OutText;
}
//------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          Slots
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_pushButtonOpenPredictorOutputFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(QString::fromStdString(PredictorOutputFolder.string()));

    if(dialog.exec())
    {
        PredictorOutputFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OpenPredictorOutputFotder();
}

void MainWindow::on_pushButtonOpenOutFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(QString::fromStdString(OutputFolder.string()));

    if(dialog.exec())
    {
        OutputFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OutputFotder();
}


void MainWindow::on_listWidgetPredictorOutFiles_currentTextChanged(const QString &currentText)
{
    ShowResult(currentText.toStdString());

}





void MainWindow::on_pushButtonProcessAllFiles_clicked()
{
    string OutText = "";
    OutText += PredictorOutputFolder.string();

    OutText += "\n";
    OutText += "File\t";
    OutText += "#Samples\t";
    OutText += "#Error\t";
    OutText += "Error[%]\n";

    ui->textEditOutFileContent->clear();

    if(PredictorOutputFileNamesVector.empty())
    {
        ui->textEditOut->append("no files to proces ");
        return;
    }
    for(vector<string>::iterator iFileNamesVector = PredictorOutputFileNamesVector.begin(); iFileNamesVector != PredictorOutputFileNamesVector.end(); iFileNamesVector++)
    {
        OutText += ShowResult(*iFileNamesVector, 0);
    }

    path OutFile = OutputFolder;
    OutFile.append(ui->lineEditFileName->text().toStdString() + ".xls");

    std::ofstream outFile (OutFile.string());
    if (!outFile.is_open())
    {
        ui->textEditOut->append("unable to open :" + QString::fromStdString(OutFile.string()));
        return;
    }
    outFile << OutText;
    outFile.close();


}
