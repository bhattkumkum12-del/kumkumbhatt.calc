#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void digitPressed();
    void operatorPressed();
    void equalsPressed();
    void clearPressed();
    void clearEntryPressed();
    void decimalPressed();
    void backspacePressed();
    void toggleSignPressed();
    void percentPressed();

private:
    QLineEdit   *m_display;
    QPushButton *m_historyLabel; // used as a small read-only "operation" indicator

    double  m_storedValue;
    QString m_pendingOperator;
    bool    m_waitingForOperand;
    bool    m_hasError;

    void setupUi();
    QPushButton *makeButton(const QString &text, const QString &objectName);
    void applyPendingOperator();
    void setDisplayValue(double value);
    void showError(const QString &message);
};

#endif // MAINWINDOW_H
