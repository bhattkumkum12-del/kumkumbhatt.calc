#include "mainwindow.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QFont>
#include <QLabel>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_display(nullptr)
    , m_historyLabel(nullptr)
    , m_storedValue(0.0)
    , m_pendingOperator(QString())
    , m_waitingForOperand(true)
    , m_hasError(false)
{
    setupUi();
}

QPushButton *MainWindow::makeButton(const QString &text, const QString &objectName)
{
    QPushButton *button = new QPushButton(text, this);
    button->setObjectName(objectName);
    button->setFixedHeight(56);
    QFont font = button->font();
    font.setPointSize(14);
    button->setFont(font);
    return button;
}

void MainWindow::setupUi()
{
    setWindowTitle("Qt Calculator");
    setFixedSize(320, 440);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(12, 12, 12, 12);
    rootLayout->setSpacing(10);

    // Display
    m_display = new QLineEdit("0", this);
    m_display->setObjectName("display");
    m_display->setReadOnly(true);
    m_display->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_display->setFixedHeight(70);
    QFont displayFont = m_display->font();
    displayFont.setPointSize(26);
    m_display->setFont(displayFont);
    m_display->setStyleSheet(
        "QLineEdit#display {"
        "  background-color: #1e1e1e;"
        "  color: #ffffff;"
        "  border-radius: 8px;"
        "  padding: 6px 12px;"
        "}"
    );
    rootLayout->addWidget(m_display);

    // Button grid
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(8);
    rootLayout->addLayout(grid);

    const QString opStyle =
        "QPushButton { background-color: #ff9500; color: white; border-radius: 8px; }"
        "QPushButton:pressed { background-color: #cc7700; }";
    const QString digitStyle =
        "QPushButton { background-color: #3a3a3c; color: white; border-radius: 8px; }"
        "QPushButton:pressed { background-color: #555557; }";
    const QString utilStyle =
        "QPushButton { background-color: #a5a5a5; color: black; border-radius: 8px; }"
        "QPushButton:pressed { background-color: #8a8a8a; }";
    const QString equalsStyle =
        "QPushButton { background-color: #34c759; color: white; border-radius: 8px; }"
        "QPushButton:pressed { background-color: #279c46; }";

    // Row 0: C, CE, %, /
    QPushButton *btnClear = makeButton("C", "btnClear");
    btnClear->setStyleSheet(utilStyle);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::clearPressed);
    grid->addWidget(btnClear, 0, 0);

    QPushButton *btnBackspace = makeButton(QString::fromUtf8("\u232B"), "btnBackspace");
    btnBackspace->setStyleSheet(utilStyle);
    connect(btnBackspace, &QPushButton::clicked, this, &MainWindow::backspacePressed);
    grid->addWidget(btnBackspace, 0, 1);

    QPushButton *btnPercent = makeButton("%", "btnPercent");
    btnPercent->setStyleSheet(utilStyle);
    connect(btnPercent, &QPushButton::clicked, this, &MainWindow::percentPressed);
    grid->addWidget(btnPercent, 0, 2);

    QPushButton *btnDivide = makeButton(QString::fromUtf8("\u00F7"), "btnDivide");
    btnDivide->setStyleSheet(opStyle);
    connect(btnDivide, &QPushButton::clicked, this, &MainWindow::operatorPressed);
    grid->addWidget(btnDivide, 0, 3);

    // Row 1: 7 8 9 *
    const QStringList row1 = {"7", "8", "9"};
    for (int i = 0; i < row1.size(); ++i) {
        QPushButton *b = makeButton(row1[i], "btnDigit" + row1[i]);
        b->setStyleSheet(digitStyle);
        connect(b, &QPushButton::clicked, this, &MainWindow::digitPressed);
        grid->addWidget(b, 1, i);
    }
    QPushButton *btnMultiply = makeButton(QString::fromUtf8("\u00D7"), "btnMultiply");
    btnMultiply->setStyleSheet(opStyle);
    connect(btnMultiply, &QPushButton::clicked, this, &MainWindow::operatorPressed);
    grid->addWidget(btnMultiply, 1, 3);

    // Row 2: 4 5 6 -
    const QStringList row2 = {"4", "5", "6"};
    for (int i = 0; i < row2.size(); ++i) {
        QPushButton *b = makeButton(row2[i], "btnDigit" + row2[i]);
        b->setStyleSheet(digitStyle);
        connect(b, &QPushButton::clicked, this, &MainWindow::digitPressed);
        grid->addWidget(b, 2, i);
    }
    QPushButton *btnMinus = makeButton("-", "btnMinus");
    btnMinus->setStyleSheet(opStyle);
    connect(btnMinus, &QPushButton::clicked, this, &MainWindow::operatorPressed);
    grid->addWidget(btnMinus, 2, 3);

    // Row 3: 1 2 3 +
    const QStringList row3 = {"1", "2", "3"};
    for (int i = 0; i < row3.size(); ++i) {
        QPushButton *b = makeButton(row3[i], "btnDigit" + row3[i]);
        b->setStyleSheet(digitStyle);
        connect(b, &QPushButton::clicked, this, &MainWindow::digitPressed);
        grid->addWidget(b, 3, i);
    }
    QPushButton *btnPlus = makeButton("+", "btnPlus");
    btnPlus->setStyleSheet(opStyle);
    connect(btnPlus, &QPushButton::clicked, this, &MainWindow::operatorPressed);
    grid->addWidget(btnPlus, 3, 3);

    // Row 4: +/-, 0, ., =
    QPushButton *btnSign = makeButton(QString::fromUtf8("\u00B1"), "btnSign");
    btnSign->setStyleSheet(digitStyle);
    connect(btnSign, &QPushButton::clicked, this, &MainWindow::toggleSignPressed);
    grid->addWidget(btnSign, 4, 0);

    QPushButton *btnZero = makeButton("0", "btnDigit0");
    btnZero->setStyleSheet(digitStyle);
    connect(btnZero, &QPushButton::clicked, this, &MainWindow::digitPressed);
    grid->addWidget(btnZero, 4, 1);

    QPushButton *btnDecimal = makeButton(".", "btnDecimal");
    btnDecimal->setStyleSheet(digitStyle);
    connect(btnDecimal, &QPushButton::clicked, this, &MainWindow::decimalPressed);
    grid->addWidget(btnDecimal, 4, 2);

    QPushButton *btnEquals = makeButton("=", "btnEquals");
    btnEquals->setStyleSheet(equalsStyle);
    connect(btnEquals, &QPushButton::clicked, this, &MainWindow::equalsPressed);
    grid->addWidget(btnEquals, 4, 3);
}

void MainWindow::setDisplayValue(double value)
{
    QString text = QString::number(value, 'g', 12);
    m_display->setText(text);
    m_waitingForOperand = true;
}

void MainWindow::showError(const QString &message)
{
    m_display->setText(message);
    m_hasError = true;
    m_waitingForOperand = true;
}

void MainWindow::digitPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button)
        return;

    if (m_hasError)
        clearPressed();

    const QString digit = button->text();

    if (m_waitingForOperand) {
        m_display->setText(digit == "0" ? "0" : digit);
        m_waitingForOperand = false;
    } else {
        if (m_display->text() == "0")
            m_display->setText(digit);
        else
            m_display->setText(m_display->text() + digit);
    }
}

void MainWindow::decimalPressed()
{
    if (m_hasError)
        clearPressed();

    if (m_waitingForOperand) {
        m_display->setText("0.");
        m_waitingForOperand = false;
        return;
    }

    if (!m_display->text().contains('.'))
        m_display->setText(m_display->text() + ".");
}

void MainWindow::operatorPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button || m_hasError)
        return;

    const QString op = button->text();

    if (!m_pendingOperator.isEmpty() && !m_waitingForOperand)
        applyPendingOperator();
    else
        m_storedValue = m_display->text().toDouble();

    m_pendingOperator = op;
    m_waitingForOperand = true;
}

void MainWindow::applyPendingOperator()
{
    const double operand = m_display->text().toDouble();
    double result = operand;

    if (m_pendingOperator == QString::fromUtf8("\u00F7")) {
        if (operand == 0.0) {
            showError("Error: divide by 0");
            m_pendingOperator.clear();
            return;
        }
        result = m_storedValue / operand;
    } else if (m_pendingOperator == QString::fromUtf8("\u00D7")) {
        result = m_storedValue * operand;
    } else if (m_pendingOperator == "-") {
        result = m_storedValue - operand;
    } else if (m_pendingOperator == "+") {
        result = m_storedValue + operand;
    } else {
        return; // no operator pending
    }

    m_storedValue = result;
    setDisplayValue(result);
}

void MainWindow::equalsPressed()
{
    if (m_hasError || m_pendingOperator.isEmpty())
        return;

    applyPendingOperator();
    m_pendingOperator.clear();
}

void MainWindow::clearPressed()
{
    m_display->setText("0");
    m_storedValue = 0.0;
    m_pendingOperator.clear();
    m_waitingForOperand = true;
    m_hasError = false;
}

void MainWindow::clearEntryPressed()
{
    m_display->setText("0");
    m_waitingForOperand = true;
}

void MainWindow::backspacePressed()
{
    if (m_hasError) {
        clearPressed();
        return;
    }

    if (m_waitingForOperand)
        return;

    QString text = m_display->text();
    text.chop(1);
    if (text.isEmpty() || text == "-")
        text = "0";
    m_display->setText(text);
}

void MainWindow::toggleSignPressed()
{
    if (m_hasError)
        return;

    double value = m_display->text().toDouble();
    setDisplayValue(-value);
    m_waitingForOperand = false;
}

void MainWindow::percentPressed()
{
    if (m_hasError)
        return;

    double value = m_display->text().toDouble();
    setDisplayValue(value / 100.0);
    m_waitingForOperand = false;
}
