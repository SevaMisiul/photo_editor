#include "photo_editor_window.h"
#include "start_dialog.h"

#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QStyleFactory>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "photo_editor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    a.setStyle(QStyleFactory::create("Fusion"));
    StartDialog startDialog;
    startDialog.setWindowTitle("New document");
    if (startDialog.exec() == QDialog::Accepted) {
        if (startDialog.getCreateMode() == StartDialog::CreateMode::create) {
            PhotoEditorWindow w(startDialog.getBackColor(),
                                startDialog.getSize(),
                                startDialog.getProjectName());
            w.show();
            return a.exec();
        } else {
            PhotoEditorWindow w(startDialog.getFilePath());
            w.show();
            return a.exec();
        }
    } else {
        return 0;
    }
}
