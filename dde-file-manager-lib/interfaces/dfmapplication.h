/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DFMAPPLICATION_H
#define DFMAPPLICATION_H

#include <QObject>

#include <dfmglobal.h>

DFM_BEGIN_NAMESPACE

class DFMSettings;
class DFMApplicationPrivate;
class DFMApplication : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DFMApplication)

public:
    // 应用级别的配置，默认存储于 ~/.config/deepin/{AppName}/dde-file-manager.json
    enum ApplicationAttribute {
        AA_AllwayOpenOnNewWindow,
        AA_IconSizeLevel,
        AA_ViewMode,
        AA_ViewComppactMode,
        AA_ViewAutoCompace,
        AA_OpenFileMode, // 点击/双击[0/1]
        AA_UrlOfNewWindow, // 新窗口默认路径
        AA_UrlOfNewTab, // 新标签页默认路径
        AA_ThemeName
    };

    Q_ENUM(ApplicationAttribute)

    // 通用型配置，默认存储于 ~/.config/deepin/dde-file-manager.json
    enum GenericAttribute {
        GA_QuickSearch, // 基于索引的文件快速搜索
        GA_PreviewCompressFile, // 把压缩包当做目录打开
        GA_PreviewTextFile, // 纯文本生成缩略图
        GA_PreviewDocumentFile, // 文档生成缩略图（pdf）
        GA_PreviewImage, // 图片生成缩略图
        GA_PreviewVideo, // 文件生成缩略图
        GA_AutoMount, // 自动挂载硬盘设备
        GA_AutoMountAndOpen, // 自动挂载并打开硬盘设备
        GA_OverrideFileChooserDialog, // 将DDE文件管理器作为应用选择文件时的对话框
        GA_ShowedHiddenOnSearch, // 搜索时显示隐藏文件
        GA_ShowedHiddenFiles, // 显示隐藏文件
        GA_ShowedFileSuffixOnRename, // 重命名文件时显示后缀
        GA_DisableNonRemovableDeviceUnmount, // 禁用本地磁盘卸载功能
        GA_HiddenSystemPartition // 隐藏系统分区
    };

    Q_ENUM(GenericAttribute)

    explicit DFMApplication(QObject *parent = nullptr);
    ~DFMApplication();

    static QVariant appAttribute(ApplicationAttribute aa);
    static DUrl appUrlAttribute(ApplicationAttribute aa);
    static void setAppAttribute(ApplicationAttribute aa, const QVariant &value);
    static bool syncAppAttribute();

    static QVariant genericAttribute(GenericAttribute ga);
    static void setGenericAttribute(GenericAttribute ga, const QVariant &value);
    static bool syncGenericAttribute();

    static DFMApplication *instance();

    static DFMSettings *genericSetting();
    static DFMSettings *appSetting();

    static DFMSettings *genericObtuselySetting();
    static DFMSettings *appObtuselySetting();

Q_SIGNALS:
    void appAttributeChanged(ApplicationAttribute aa, const QVariant &value);
    void genericAttributeChanged(GenericAttribute ga, const QVariant &value);
    void appAttributeEdited(ApplicationAttribute aa, const QVariant &value);
    void genericAttributeEdited(GenericAttribute ga, const QVariant &value);
    void iconSizeLevelChanged(int level);
    void viewModeChanged(int mode);
    void previewCompressFileChanged(bool enable);
    void previewAttributeChanged(GenericAttribute ga, bool enable);
    void showedHiddenFilesChanged(bool enable);

    void genericSettingCreated(DFMSettings *settings);
    void appSettingCreated(DFMSettings *settings);

protected:
    DFMApplication(DFMApplicationPrivate *dd, QObject *parent = nullptr);

private:
    QScopedPointer<DFMApplicationPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_ptr, void _q_onSettingsValueChanged(const QString&, const QString &, const QVariant &))
    Q_PRIVATE_SLOT(d_ptr, void _q_onSettingsValueEdited(const QString&, const QString &, const QVariant &))
};

DFM_END_NAMESPACE

#endif // DFMAPPLICATION_H