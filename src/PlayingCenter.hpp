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
#include <QTimer>
#include <PlayListDTOs.hpp>
#include "ModuleHandler.hpp"
#include <Interfaces/Player.hpp>
#include <Interfaces/PlayerSignals.hpp>
#include <Interfaces/ModulePlayer.hpp>

namespace ModPlugPlayer {
    class PlayingCenter : public QObject {
        Q_OBJECT
    public:
        PlayingCenter(PlayingCenter const &) = delete;
        void operator =(PlayingCenter const &) = delete;
        static PlayingCenter& getInstance();
        double getVUMeterValue();
        void getSpectrumData(double * spectrumData);
        PlayingState getPlayingState();

        ~PlayingCenter();

             //     static PLAYERSTATE playerState;
             //     static SONGSTATE songState;

    public slots:
        // Request Signal Handlers
        void onOpenRequested();
        void onOpenRequested(const std::filesystem::path filePath);
        void onStopRequested();
        void onStopRequested(const ModPlugPlayer::SongFileInfo songFileInfo);
        void onStopRequested(const ModPlugPlayer::PlayListItem playListItem);
        void onPlayRequested();
        void onPlayRequested(const ModPlugPlayer::SongFileInfo songFileInfo);
        void onPlayRequested(const ModPlugPlayer::PlayListItem playListItem);
        void onPauseRequested();
        void onPauseRequested(const ModPlugPlayer::SongFileInfo songFileInfo);
        void onPauseRequested(const ModPlugPlayer::PlayListItem playListItem);
        void onResumeRequested();
        void onResumeRequested(const ModPlugPlayer::SongFileInfo songFileInfo);
        void onResumeRequested(const ModPlugPlayer::PlayListItem playListItem);
        void onVolumeChangeRequested(const int volume);
        void onScrubbingRequested(const int position);
        void onScrubberUpdatePeriodChangeRequested(const unsigned int updatePeriod);
        void onRewindRequested();
        void onFastForwardRequested();
        void onRepeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode);
        void onEqStateChangeRequested(const bool activated);
        void onDSPStateChangeRequested(const bool activated);
        void onAmigaFilterChangeRequested(const AmigaFilter amigaFilter);
        void onInterpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter);
        void onSpectrumAnalyzerWindowFunctionChanged(const ModPlugPlayer::WindowFunction windowFunction);
        void onOutputDeviceChangeRequested(const int outputDeviceIndex);
        void onSoundResolutionChangeRequested(const SampleRate sampleRate, const BitRate bitRate, const ChannelMode channelMode);

    private slots:
        //Request Signal Handlers
        void repeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode);

        //Response Signal Handlers
        void onLoaded(const ModPlugPlayer::SongFileInfo songFileInfo, const bool successfull);
        void onLoaded(const ModPlugPlayer::PlayListItem playListItem, bool successfull);
        void onPlayingStarted();
        void onPlayingStarted(const ModPlugPlayer::SongFileInfo songFileInfo);
        void onPlayingStarted(const ModPlugPlayer::PlayListItem playListItem);
        void onStopped();
        void onStopped(const ModPlugPlayer::SongFileInfo songFileInfo);
        void onStopped(const ModPlugPlayer::PlayListItem playListItem);
        void onPaused();
        void onPaused(const ModPlugPlayer::SongFileInfo songFileInfo);
        void onPaused(const ModPlugPlayer::PlayListItem playListItem);
        void onResumed();
        void onResumed(const ModPlugPlayer::SongFileInfo songFileInfo);
        void onResumed(const ModPlugPlayer::PlayListItem playListItem);
        void onRepeatModeChanged(const ModPlugPlayer::RepeatMode repeatMode);

        void updateScrubber();
    private:
        explicit PlayingCenter(QObject *parent = nullptr);
        QFileDialog *fileDialog = nullptr;
        QTimer *scrubberTimer = nullptr;
        QTimer *instantModuleInfoTimer = nullptr;
        QTimer *timeInfoTimer = nullptr;
        ModuleHandler moduleHandler;
        PlayingMode playingMode = PlayingMode::Song;
        SongFileInfo currentSongFileInfo; //loaded module file info
        PlayListItem currentPlayListItem; //loaded playlist item info
        size_t previousActiveChannelAmount = 0;
        size_t previousSubSongIndex = 0;
        size_t previousPatternAmount = 0;
        size_t previousPatternIndex = 0;
        int previousElapsedSeconds = 0;
        size_t currentActiveChannelAmount = 0;
        size_t currentSubSongIndex = 0;
        size_t currentPatternAmount = 0;
        size_t currentPatternIndex = 0;
        int currentElapsedSeconds = 0;

        SampleRate sampleRate = SampleRate::Hz44100;
        BitRate bitRate = BitRate::Bits16;
        ChannelMode channelMode = ChannelMode::Stereo;

        void connectSignalsAndSlots();
        void updateInstantModuleInfo();
        void updateTimeInfo();
        QString getSupportedExtensionsAsString();

        QString getLessKnownSupportedExtensionsAsString();
        void afterLoaded(const SongFileInfo fileInfo);
        unsigned int previousScrubberPosition = 0;
        unsigned int currentScrubberPosition = 0;
    };
}
