#ifndef QCOLORVALIDATOR_H
#define QCOLORVALIDATOR_H

#include <QIntValidator>

class QColorValidator : public QIntValidator
{
public:
    explicit QColorValidator(QObject *parent = nullptr);

    // QValidator interface
public:
    virtual State validate(QString &, int &) const override;
};

#endif // QCOLORVALIDATOR_H
