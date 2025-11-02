#include <QCoreApplication>
#include <QtGlobal>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "=== Qt Module Check ===";
    qDebug() << "Qt version:" << QT_VERSION_STR;

#ifdef QT_WEBENGINEWIDGETS_LIB
    qDebug() << "✓ WebEngineWidgets: AVAILABLE";
#else
    qDebug() << "✗ WebEngineWidgets: NOT AVAILABLE";
#endif

#ifdef QT_WEBENGINE_LIB
    qDebug() << "✓ WebEngine: AVAILABLE";
#else
    qDebug() << "✗ WebEngine: NOT AVAILABLE";
#endif

#ifdef QT_PRINTSUPPORT_LIB
    qDebug() << "✓ PrintSupport: AVAILABLE";
#else
    qDebug() << "✗ PrintSupport: NOT AVAILABLE";
#endif

    return 0;
}
