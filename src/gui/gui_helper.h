#ifndef GUI_GUI_HELPER_H
#define GUI_GUI_HELPER_H

#include <QFileIconProvider>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QString>

namespace gui_helper {
    // Helper to create a layout containing an icon and a label
    inline QHBoxLayout* CreateLabelIconGrpLayout(QLabel* label, QLabel* label_icon) {
        auto* layout_grp = new QHBoxLayout();
        layout_grp->setSpacing(1);
        layout_grp->setContentsMargins(0, 0, 0, 0);
        layout_grp->setAlignment(Qt::AlignLeft);
        layout_grp->addWidget(label_icon);
        layout_grp->addWidget(label);
        return layout_grp;
    }

    /**
     * Constructs and returns a layout containing an icon and a label using icon file
     * @return QHBoxLayout* The generated layout containing the widgets.
     */
    inline QHBoxLayout* CreateLabelIconGrp(
        QLabel* label,
        int icon_size,
        const QString& icon_path,
        const QString& object_name = "",
        QWidget* parent = nullptr
    ) {
        if (!label) return nullptr;

        auto* label_icon = new QLabel(parent);
        label_icon->setObjectName(object_name);
        label_icon->setPixmap(QIcon(icon_path).pixmap(icon_size, icon_size));

        return CreateLabelIconGrpLayout(label, label_icon);
    }

    /**
     * Constructs and returns a layout containing an icon and a label using QFileIconProvider
     * @return QHBoxLayout* The generated layout containing the widgets.
     */
    inline QHBoxLayout* CreateLabelIconGrp(
        QLabel* label,
        int icon_size,
        QFileIconProvider::IconType icon_type,
        const QString& object_name = "",
        QWidget* parent = nullptr
    ) {
        if (!label) return nullptr;

        auto* label_icon = new QLabel(parent);
        label_icon->setObjectName(object_name);
        label_icon->setPixmap(QFileIconProvider().icon(icon_type).pixmap(icon_size, icon_size));

        return CreateLabelIconGrpLayout(label, label_icon);
    }
}  // namespace gui_helper

#endif  // GUI_GUI_HELPER_H