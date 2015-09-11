/*
 * Copyright (C) 2013 Emeric Poupon
 *
 * This file is part of LMS.
 *
 * LMS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LMS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LMS.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <boost/foreach.hpp>

#include <Wt/WMediaPlayer>

#include "VideoParametersDialog.hpp"

#include "VideoMediaPlayerWidget.hpp"

namespace UserInterface {

Wt::WMediaPlayer::Encoding
AvEncoding_to_WtEncoding(Av::Encoding encoding)
{
	switch( encoding )
	{
		case Av::Encoding::OGA: return Wt::WMediaPlayer::OGA;
		case Av::Encoding::OGV: return Wt::WMediaPlayer::OGV;
		case Av::Encoding::MP3: return Wt::WMediaPlayer::MP3;
		case Av::Encoding::WEBMA: return Wt::WMediaPlayer::WEBMA;
		case Av::Encoding::WEBMV: return Wt::WMediaPlayer::WEBMV;
		case Av::Encoding::FLA: return Wt::WMediaPlayer::FLA;
		case Av::Encoding::FLV: return Wt::WMediaPlayer::FLV;
		case Av::Encoding::M4A: return Wt::WMediaPlayer::M4A;
		case Av::Encoding::M4V: return Wt::WMediaPlayer::M4V;
	}
	assert(0);
}


VideoMediaPlayerWidget::VideoMediaPlayerWidget( Av::TranscodeParameters parameters, Wt::WContainerWidget *parent)
	: Wt::WContainerWidget(parent),
	_mediaResource(nullptr),
	_currentParameters(parameters),
	_dialog(nullptr)
{
	_mediaPlayer = new Wt::WMediaPlayer( Wt::WMediaPlayer::Video, this );
//	_mediaPlayer->setAlternativeContent (new Wt::WText("You don't have HTML5 audio support!"));
//	_mediaPlayer->setOptions( Wt::WMediaPlayer::Autoplay );
//	_mediaPlayer->addSource( Wt::WMediaPlayer::WEBMV, "" );

	{
		Wt::WContainerWidget *container = new Wt::WContainerWidget(this);

		_playBtn = new Wt::WPushButton("Play", container );
		_pauseBtn = new Wt::WPushButton("Pause", container );

		_curTime = new Wt::WText(container);
		_timeSlider = new Wt::WSlider( container );
		_duration = new Wt::WText(container);

		_volumeSlider = new Wt::WSlider( container );
		_volumeSlider->setRange(0,100);
		_volumeSlider->setValue(_mediaPlayer->volume() * 100);

		_mediaPlayer->setControlsWidget( container );
		_mediaPlayer->setButton(Wt::WMediaPlayer::Play, _playBtn);
		_mediaPlayer->setButton(Wt::WMediaPlayer::Pause, _pauseBtn);

		_mediaPlayer->setText( Wt::WMediaPlayer::CurrentTime, _curTime);
		_mediaPlayer->setText( Wt::WMediaPlayer::Duration, _duration);

		_mediaPlayer->timeUpdated().connect(this, &VideoMediaPlayerWidget::handleTimeUpdated);

		Wt::WPushButton* fullScreenButton = new Wt::WPushButton("Fullscreen", container);
		_mediaPlayer->setButton(Wt::WMediaPlayer::FullScreen, fullScreenButton);

		Wt::WPushButton* restoreScreenButton = new Wt::WPushButton("Restore screen", container);
		_mediaPlayer->setButton(Wt::WMediaPlayer::RestoreScreen, restoreScreenButton);

		_mediaPlayer->timeUpdated().connect(this, &VideoMediaPlayerWidget::handleTimeUpdated);

		_timeSlider->valueChanged().connect(this, &VideoMediaPlayerWidget::handlePlayOffset);
		_timeSlider->sliderMoved().connect(this, &VideoMediaPlayerWidget::handleSliderMoved);

		_volumeSlider->sliderMoved().connect(this, &VideoMediaPlayerWidget::handleVolumeSliderMoved);
	}


	Wt::WPushButton* closeButton = new Wt::WPushButton("Close", this);
	closeButton->clicked().connect( this, &VideoMediaPlayerWidget::handleClose );

	Wt::WPushButton* parametersButton = new Wt::WPushButton("Parameters", this);
	parametersButton->clicked().connect( this, &VideoMediaPlayerWidget::handleParametersEdit );

	load(parameters);
}

void
VideoMediaPlayerWidget::load(Av::TranscodeParameters parameters)
{
	_mediaPlayer->clearSources();

	_currentParameters = parameters;

	_mediaInternalLink.setResource( nullptr );
	if (_mediaResource)
		delete _mediaResource;

// TODO 	_mediaResource = new AvConvTranscodeStreamResource( parameters, this );
	_mediaInternalLink.setResource( _mediaResource );

	_mediaPlayer->addSource( AvEncoding_to_WtEncoding(parameters.getEncoding()), _mediaInternalLink );

	_timeSlider->setRange(0, 0/* TODO  parameters.getInputMediaFile().getDuration().total_seconds()*/ );
	_timeSlider->setValue( parameters.getOffset().total_seconds() );

// TODO	_duration->setText( boost::posix_time::to_simple_string( parameters.getInputMediaFile().getDuration() ));

	_mediaPlayer->play();
}

void
VideoMediaPlayerWidget::handlePlayOffset(int offsetSecs)
{
	std::cout << "Want to play at offset " << offsetSecs << std::endl;;
	_currentParameters.setOffset( boost::posix_time::seconds(offsetSecs) );
	load( _currentParameters );

	_timeSlider->setValue( offsetSecs );
}

void
VideoMediaPlayerWidget::handleSliderMoved(int value)
{
	std::cout << "Slider moved to " << value << std::endl;
	_curTime->setText( boost::posix_time::to_simple_string( boost::posix_time::seconds( value ) ) );
}

void
VideoMediaPlayerWidget::handleTimeUpdated(void)
{
	std::cout << "Time updated to " << _mediaPlayer->currentTime() << std::endl;

	/* TODO
	if (_mediaPlayer->currentTime() > 0 && _mediaPlayer->currentTime() < _currentParameters.getInputMediaFile().getDuration().total_seconds())
	{
		boost::posix_time::time_duration currentTime ( boost::posix_time::seconds( _mediaPlayer->currentTime() + _currentParameters.getOffset().total_seconds()));

		_timeSlider->setValue( currentTime.total_seconds() );
		_curTime->setText( boost::posix_time::to_simple_string( currentTime) );
	}
	*/
}

void
VideoMediaPlayerWidget::handleVolumeSliderMoved(int value)
{
	_mediaPlayer->setVolume( value / 100. );
}

void
VideoMediaPlayerWidget::handleClose(void)
{
	_close.emit();
}

void
VideoMediaPlayerWidget::handleParametersEdit(void)
{

//	_dialog = new VideoParametersDialog("Parameters");
//TODO	_dialog->load(_currentParameters);

//	_dialog->show();

//	_dialog->finished().connect(this, &VideoMediaPlayerWidget::handleParametersDone);
}

void
VideoMediaPlayerWidget::handleParametersDone(Wt::WDialog::DialogCode code)
{
	assert(_dialog != nullptr);

	if (code == Wt::WDialog::Accepted)
	{
// TODO		_dialog->save( _currentParameters );

		// TODO SYNC current offset with player?
		// HACK use slider current value
		handlePlayOffset( _timeSlider->value());
	}

}

} // namespace UserInterface
