#include "widget.h"
#include "ui_widget.h"
#include <QRegExp>
#include <QClipboard>
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::weed(QString &str)
{
    QRegExp re("(virtual\\s+)?([a-zA-Z]*[ ]*[&*~_a-zA-Z]*\\([_=&*,:a-zA-Z 0-9]*\\)(\\s+const)?(\\s+override)?;\n)");
    QString tmp = str;
    str = "";

    int count = 0;
    int pos = 0;
    while ((pos = re.indexIn(tmp, pos)) != -1)
    {
        ++count;
        pos += re.matchedLength();
        str += re.cap(0);
    }
}

void Widget::replaceSemicolon(QString &str)
{
    str = str.replace(";", "\n{\n\n}\n");
}

void Widget::addClassName(QString &str, const QString &name)
{
    QRegExp re("(virtual\\s+)?([a-zA-Z]*[ ]*)([&*~_a-zA-Z]*)(\\([_=&*,:a-zA-Z 0-9]*\\)(\\s+const)?(\\s+override)?;\n)");
//    QRegExp re("([a-zA-Z]*[ ]*)([&*~_a-zA-Z])(*\\([_=&*,:a-zA-Z 0-9]*\\);\n)");
    QString tmp = str;
    str = "";

    int count = 0;
    int pos = 0;
    while ((pos = re.indexIn(tmp, pos)) != -1)
    {
        ++count;
        pos += re.matchedLength();
        QString funcName = re.cap(3);
        funcName.push_front(name + "::");
        str += re.cap(2) + funcName + re.cap(4) + re.cap(5);
    }
}

QString Widget::genDefine(const QString &str)
{
    QString txt = str;
    QString name = parseClassName(txt);
    weed(txt);
    addClassName(txt, name);
    replaceSemicolon(txt);
    ui->editResult->setText(txt);

    return txt;
}

QString Widget::parseClassName(const QString &str)
{
    QRegExp re("class ([a-zA-Z0-9_]+)");

    int pos = re.indexIn(str);
    if (pos < 0) return nullptr;

    QString name = re.cap(1);
    return name;
}

QString Widget::genHeadTemplate() const
{
    QString clsName = ui->editOrigin->toPlainText();
    QString headTemp = R"(
                       class EntityCardHead : public cs::CSWidget
                       {
                           Q_OBJECT

                       public:
                           EntityCardHead(QWidget *parent = nullptr);
                           virtual ~EntityCardHead() override;

                       protected:
                           void initMember();
                           void initUI();

                       protected:
                           void resizeUI();

                       protected:
                           void resizeEvent(QResizeEvent *event) override;

                       };
                       )";
    headTemp = headTemp.replace("EntityCardHead", clsName);

    ui->editResult->setText(headTemp);
    return headTemp;
}

// Generate resource alias
QString Widget::parseResourceName(const QString &str)
{
    QRegExp re("^\\s*(<file.*>)(.*/)*(.*)\\.(png|jpg|jpeg|qss|json)</file>\\s*$");

    int pos = re.indexIn(str);
    if (pos < 0) return nullptr;

    QString name = re.cap(3);
    weedResourceAlias(name);
    return name;
}

void Widget::weedResourceAlias(QString &str)
{
    // Weed suffix likes `_32x32`
    QRegExp re("^(.*)_([0-9]+x[0-9]+)$");

    int pos = re.indexIn(str);
    if (pos < 0) return;

    str = re.cap(1);
    replaceUnderline(str);
}

void Widget::replaceUnderline(QString &str)
{
    str = str.replace("_", "-");
}

void Widget::genResourceAlias()
{
    auto str = ui->editOrigin->toPlainText();
    auto strList = str.split("\n");
    QString res;

    for (auto line : strList)
    {
        res += parseResourceName(line) + "\n";
    }

    ui->editResult->setText(res);
}

void Widget::on_btnGen_clicked()
{
    QString originTxt = ui->editOrigin->toPlainText();
    QString resultTxt = genDefine(originTxt);
    ui->editResult->setText(resultTxt);
}

void Widget::on_btnClearAll_clicked()
{
    ui->editOrigin->clear();
    ui->editResult->clear();
}

void Widget::on_btnCopyRes_clicked()
{
    QString str = ui->editResult->toPlainText();
    QApplication::clipboard()->setText(str);
}

void Widget::on_btnPaste_clicked()
{
    QString str = QApplication::clipboard()->text();
    ui->editOrigin->setText(str);
}

void Widget::on_btnGenius_clicked()
{
    on_btnClearAll_clicked();
    on_btnPaste_clicked();
    on_btnGen_clicked();
    on_btnCopyRes_clicked();
}

void Widget::on_btnResPath_clicked()
{
    on_btnPaste_clicked();
    genResourceAlias();
    on_btnCopyRes_clicked();
}

void Widget::on_btnHeadTemp_clicked()
{
    on_btnClearAll_clicked();
    on_btnPaste_clicked();
    genHeadTemplate();
    on_btnCopyRes_clicked();
}
