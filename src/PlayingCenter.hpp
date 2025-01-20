/*
PlayingCenter class declarations of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <QObject>
#include <QFileDialog>
#include <PlayListDTOs.hpp>
#include "ModuleHandler.hpp"
#include <Interfaces/Player.hpp>
#include <Interfaces/PlayerSignals.hpp>
#include <Interfaces/ModulePlayer.hpp>

namespace ModPlugPlayer {
    class PlayingCenter : public QObject,
                      public ModPlugPlayer::Interfaces::Player,
                      public ModPlugPlayer::Interfaces::PlayerSignals,
                      public ModPlugPlayer::Interfaces::ModulePlayer

    {
        Q_OBJECT
    public:
        explicit PlayingCenter(QObject *parent = nullptr);
        ~PlayingCenter();

             //     static PLAYERSTATE playerState;
             //     static SONGSTATE songState;

        //Player Controls
        int getVolume() const override;
        void setVolume(int volume) override;
        bool isAlwaysOnTop() const override;
        bool isSnapToViewPort() const override;
        bool isKeptStayingInViewPort() const override;
        bool isTitleBarHidden() const override;

    signals:
        //Request Signals
        void amigaFilterChangeRequested(const AmigaFilter amigaFilter) override;
        void interpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter) override;

        //Response Signals

        //Module Player Signals
        //Response Signals
        void loaded(const SongFileInfo songFileInfo, bool successfull) override;
        void loaded(const PlayListItem playListItem, bool successfull) override;
        void stopped() override;
        void stopped(const PlayListItem playListItem) override;
        void playingStarted() override;
        void playingStarted(const SongFileInfo songFileInfo) override;
        void playingStarted(const PlayListItem playListItem) override;
        void paused() override;
        void paused(const SongFileInfo songFileInfo) override;
        void paused(const PlayListItem playListItem) override;
        void resumed() override;
        void resumed(const SongFileInfo songFileInfo) override;
        void resumed(const PlayListItem playListItem) override;
        void previous() override;
        void next() override;
        void volumeChanged(const int volume) override;
        void timeScrubbed(const int position) override;
        void repeatModeChanged(const RepeatMode repeat) override;
        void eqStateChanged(const bool activated) override;
        void dspStateChanged(const bool activated) override;
        void alwaysOnTopStateChanged(const bool alwaysOnTop) override;
        void titleBarHidingStateChanged(const bool hide) override;
        void snappingToViewPortStateChanged(const bool snapToViewPort) override;
        void keepingStayingInViewPortStateChanged(const bool toBeKeptStayingInViewPort) override;
        void amigaFilterChanged(const AmigaFilter amigaFilter) override;
        void interpolationFilterChanged(const InterpolationFilter interpolationFilter) override;

    public slots:
        // Request Signal Handlers
        void onOpenRequested() override;
        void onOpenRequested(const std::filesystem::path filePath) override;
        void onStopRequested() override;
        void onStopRequested(const SongFileInfo songFileInfo) override;
        void onStopRequested(const PlayListItem playListItem) override;
        void onPlayRequested() override;
        void onPlayRequested(const SongFileInfo songFileInfo) override;
        void onPlayRequested(const PlayListItem playListItem) override;
        void onPauseRequested() override;
        void onPauseRequested(const SongFileInfo songFileInfo) override;
        void onPauseRequested(const PlayListItem playListItem) override;
        void onResumeRequested() override;
        void onResumeRequested(const SongFileInfo songFileInfo) override;
        void onResumeRequested(const PlayListItem playListItem) override;
        void onVolumeChangeRequested(const int volume) override;
        void onTimeScrubbingRequested(const int position) override;
        void onTimeScrubbed(const int position) override;
        void onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop) override;
        void onTitleBarHidingStateChangeRequested(const bool hide) override;
        void onSnappingToViewPortStateChangeRequested(const bool snapToViewPort) override;
        void onSnappingThresholdChangeRequested(const int snappingThreshold) override;
        void onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort) override;
        void onPreviousRequested() override;
        void onPreviousRequested(const PlayListItem playListItem) override;
        void onNextRequested() override;
        void onNextRequested(const PlayListItem playListItem) override;
        void onRewindRequested() override;
        void onFastForwardRequested() override;
        void onRepeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode) override;
        void onEqStateChangeRequested(const bool activated) override;
        void onDSPStateChangeRequested(const bool activated) override;
        void onAmigaFilterChangeRequested(const AmigaFilter amigaFilter) override;
        void onInterpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter) override;
        void onSetupRequested() override;

        //Response Signal Handlers
        void onLoaded(const SongFileInfo songFileInfo, const bool successfull) override;
        void onLoaded(const PlayListItem playListItem, bool successfull) override;
        void onPlayingStarted() override;
        void onPlayingStarted(const SongFileInfo songFileInfo) override;
        void onPlayingStarted(const PlayListItem playListItem) override;
        void onStopped() override;
        void onStopped(const SongFileInfo songFileInfo) override;
        void onStopped(const PlayListItem playListItem) override;
        void onPaused() override;
        void onPaused(const SongFileInfo songFileInfo) override;
        void onPaused(const PlayListItem playListItem) override;
        void onResumed() override;
        void onResumed(const SongFileInfo songFileInfo) override;
        void onResumed(const PlayListItem playListItem) override;
        void onRepeatModeChanged(const RepeatMode repeatMode) override;
        void onAmigaFilterChanged(const AmigaFilter amigaFilter) override;
        void onInterpolationFilterChanged(const InterpolationFilter interpolationFilter) override;
    private:
        QFileDialog *fileDialog = nullptr;
        ModuleHandler moduleHandler;
        PlayingMode playingMode = PlayingMode::SingleTrack;
        SongFileInfo currentSongFileInfo; //loaded module file info
        PlayListItem currentPlayListItem; //loaded playlist item info
        void connectSignalsAndSlots();
        void updateInstantModuleInfo();
        QString getSupportedExtensionsAsString();
        QString getLessKnownSupportedExtensionsAsString();
    };
}
