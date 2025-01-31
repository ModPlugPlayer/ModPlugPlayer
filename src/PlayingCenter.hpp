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
    class PlayingCenter : public QObject {
        Q_OBJECT
    public:
        explicit PlayingCenter(QObject *parent = nullptr);
        ~PlayingCenter();

             //     static PLAYERSTATE playerState;
             //     static SONGSTATE songState;

        //Player Controls
        int getVolume() const;
        void setVolume(int volume);
        bool isAlwaysOnTop() const;
        bool isSnapToViewPort() const;
        bool isKeptStayingInViewPort() const;
        bool isTitleBarHidden() const;

    public slots:
        // Request Signal Handlers
        void onOpenRequested();
        void onOpenRequested(const std::filesystem::path filePath);
        void onStopRequested();
        void onStopRequested(const SongFileInfo songFileInfo);
        void onStopRequested(const PlayListItem playListItem);
        void onPlayRequested();
        void onPlayRequested(const SongFileInfo songFileInfo);
        void onPlayRequested(const PlayListItem playListItem);
        void onPauseRequested();
        void onPauseRequested(const SongFileInfo songFileInfo);
        void onPauseRequested(const PlayListItem playListItem);
        void onResumeRequested();
        void onResumeRequested(const SongFileInfo songFileInfo);
        void onResumeRequested(const PlayListItem playListItem);
        void onVolumeChangeRequested(const int volume);
        void onTimeScrubbingRequested(const int position);
        void onTimeScrubbed(const int position);
        void onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop);
        void onTitleBarHidingStateChangeRequested(const bool hide);
        void onSnappingToViewPortStateChangeRequested(const bool snapToViewPort);
        void onSnappingThresholdChangeRequested(const int snappingThreshold);
        void onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort);
        void onPreviousRequested();
        void onPreviousRequested(const PlayListItem playListItem);
        void onNextRequested();
        void onNextRequested(const PlayListItem playListItem);
        void onRewindRequested();
        void onFastForwardRequested();
        void onRepeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode);
        void onEqStateChangeRequested(const bool activated);
        void onDSPStateChangeRequested(const bool activated);
        void onAmigaFilterChangeRequested(const AmigaFilter amigaFilter);
        void onInterpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter);
        void onSetupRequested();

        //Response Signal Handlers
        void onLoaded(const SongFileInfo songFileInfo, const bool successfull);
        void onLoaded(const PlayListItem playListItem, bool successfull);
        void onPlayingStarted();
        void onPlayingStarted(const SongFileInfo songFileInfo);
        void onPlayingStarted(const PlayListItem playListItem);
        void onStopped();
        void onStopped(const SongFileInfo songFileInfo);
        void onStopped(const PlayListItem playListItem);
        void onPaused();
        void onPaused(const SongFileInfo songFileInfo);
        void onPaused(const PlayListItem playListItem);
        void onResumed();
        void onResumed(const SongFileInfo songFileInfo);
        void onResumed(const PlayListItem playListItem);
        void onRepeatModeChanged(const RepeatMode repeatMode);
        void onAmigaFilterChanged(const AmigaFilter amigaFilter);
        void onInterpolationFilterChanged(const InterpolationFilter interpolationFilter);
    private:
        QFileDialog *fileDialog = nullptr;
        ModuleHandler moduleHandler;
        PlayingMode playingMode = PlayingMode::SingleTrack;
        SongFileInfo currentSongFileInfo; //loaded module file info
        PlayListItem currentPlayListItem; //loaded playlist item info
        size_t previousActiveChannelAmount = 0;
        size_t previousSubSongIndex = 0;
        size_t previousPatternAmount = 0;
        size_t previousPatternIndex = 0;
        size_t currentActiveChannelAmount = 0;
        size_t currentSubSongIndex = 0;
        size_t currentPatternAmount = 0;
        size_t currentPatternIndex = 0;
        void connectSignalsAndSlots();
        void updateInstantModuleInfo();
        QString getSupportedExtensionsAsString();
        QString getLessKnownSupportedExtensionsAsString();
    };
}
