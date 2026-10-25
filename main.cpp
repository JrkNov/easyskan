#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // --- AUTOMATICKÝ REŽIM VÍCEJAZYČNÉ LOKALIZACE (Qt i18n) ---
    // Program se podívá do systému Fedory, a pokud najde angličtinu (nebo jiný jazyk),
    // automaticky načte binární překladový soubor .qm z adresáře aplikace.
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        // Hledáme soubor ve tvaru např. easyskan_en_US.qm přímo v build složce
        if (translator.load(QLocale(locale), "easyskan", "_", app.applicationDirPath())) {
            app.installTranslator(&translator);
            break;
        }
    }

    // Nastavení globálních parametrů aplikace pro desktopové prostředí KDE
    app.setApplicationName("easyskan");
    app.setApplicationDisplayName("EasySkan - Rychlé studiové skenování 1:1");
    app.setOrganizationName("JirkaStudio");

    // Inicializujeme hlavní okno naší skenovací linky
    MainWindow mainWin;

    // Nekompromisní start na plnou plochu monitoru podle tvého zadání!
    mainWin.showMaximized();

    return app.exec();
}
