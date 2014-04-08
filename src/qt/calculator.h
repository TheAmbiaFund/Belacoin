#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QTimer>

namespace Ui {
    class CalculatorPage;
}
class ClientModel;
class WalletModel;
class CalculatorViewDelegate;

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** calculator page widget */
class CalculatorPage : public QWidget
{
    Q_OBJECT

public:
    explicit CalculatorPage(QWidget *parent = 0);
    ~CalculatorPage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);

public slots:

signals:

private:
    Ui::CalculatorPage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;

    CalculatorViewDelegate *calculatordelegate;
    QNetworkAccessManager *nam;

private slots:
    void finished(QNetworkReply *reply);
    void DoHttpGet();
};

#endif // CALCULATOR_H
