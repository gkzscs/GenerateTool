#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    // For class declaration
    void weed(QString &str);
    void replaceSemicolon(QString &str);
    void addClassName(QString &str, const QString &name);
    QString genDefine(const QString &str);
    QString parseClassName(const QString &str);
    QString genHeadTemplate() const;

private:
    // For resource path
    QString parseResourceName(const QString &str);
    void weedResourceAlias(QString &str);
    void replaceUnderline(QString &str);
    void genResourceAlias();

private slots:
    void on_btnGen_clicked();

    void on_btnClearAll_clicked();

    void on_btnCopyRes_clicked();

    void on_btnPaste_clicked();

    void on_btnGenius_clicked();

    void on_btnResPath_clicked();

    void on_btnHeadTemp_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
