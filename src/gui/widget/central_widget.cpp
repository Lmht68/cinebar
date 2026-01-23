#include "gui/widget/central_widget.h"

#include "gui/gui_helper.h"
#include "gui/view/barcode_view.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent) {
    // Init central widget layout
    auto* layout_main = new QVBoxLayout(this);
    layout_main->setSpacing(0);
    setLayout(layout_main);

    view_barcode_ = new BarcodeView(this);
    layout_main->addWidget(view_barcode_, 1);
    // Create a widget for zoom level, dimension, and fit to screen button
    widget_view_info_ = new QWidget(this);
    auto* layout_view_info = new QHBoxLayout(widget_view_info_);
    widget_view_info_->setLayout(layout_view_info);
    layout_main->addWidget(widget_view_info_);

    widget_view_info_->setObjectName("widgetViewInfo");
    layout_view_info->setContentsMargins(10, 10, 10, 10);
    layout_view_info->setSpacing(15);
    // Initially hide the info widget
    widget_view_info_->hide();

    label_zoom_level_ = new QLabel("", this);
    label_image_dimension_ = new QLabel("", this);
    btn_fit_to_screen_ = new QPushButton("", this);
    btn_fit_to_screen_->setIcon(QIcon(":/icons/fit-to-screen.png"));
    btn_fit_to_screen_->setIconSize(QSize(18, 18));
    btn_fit_to_screen_->setToolTip("Fit to Screen");

    label_zoom_level_->setObjectName("labelViewInfo");
    label_image_dimension_->setObjectName("labelViewInfo");
    btn_fit_to_screen_->setObjectName("btnViewInfo");

    layout_view_info->addLayout(
        gui_helper::CreateLabelIconGrp(
            label_image_dimension_, 18, ":/icons/dimension.png", "labelIcon"
        )
    );
    layout_view_info->addStretch();
    layout_view_info->addLayout(
        gui_helper::CreateLabelIconGrp(label_zoom_level_, 18, ":/icons/zoom.png", "labelIcon")
    );
    layout_view_info->addWidget(btn_fit_to_screen_);

    connect(btn_fit_to_screen_, &QPushButton::clicked, this, &CentralWidget::OnFitToScreenClicked);
    connect(view_barcode_, &BarcodeView::ZoomLevelChanged, this, &CentralWidget::UpdateImageInfo);
}

void CentralWidget::DisplayBarcode(const QPixmap& pixmap) {
    view_barcode_->DisplayImage(pixmap);

    if (pixmap.isNull()) {
        widget_view_info_->hide();
    } else {
        widget_view_info_->show();
    }
}

void CentralWidget::OnFitToScreenClicked() { view_barcode_->FitToScreen(); }

void CentralWidget::UpdateImageInfo(double zoom_level, const QSize& image_size) {
    label_zoom_level_->setText(QString("%1%").arg(static_cast<int>(zoom_level)));
    label_image_dimension_->setText(
        QString("%1 x %2").arg(image_size.width()).arg(image_size.height())
    );
}