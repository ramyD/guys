Guys
=========
version 1.0 (yay!!)

Guys is command line utility used to keep tabs on your social networky accounts, although for now, only Twitter is supported. If any of our users (all 2 of them) want other services to be added, they just need to ask.
Developpers are more likely to push features people will use and enjoy.
You can [reach me](http://github.com/ramyD) and just, you know, ask!

This project was started as trek, not a destination.

How to use
------------
Guys is pretty simple to use, from the command line, you can see your twitter timeline simply by typping:

    guys -g

this will print out your last 20 updates into your terminal.

to post you can type either of these:

    guys -t this is my post
    guys -t
    &#42&#42your Post: this is my post

When you first run Guys, you will be asked for your password, this is only to create your OAuth access token which will be saved under ~/.config/guys/twitter
If you feel your tokens have been compromised or simply wish to generate new ones, you can delete the twitter file and a new token will be generated the next time you use guys
Alternatively you can always block complete access from guys to your twitter acount form your twitter settings.

Although your password is not storred in the application or on the computer at any time, someone can take the access tokens generated and saved in your twitter file and, combined with Guys' consumer key and "secret" which we fully disclose out of futility, can access your account through the api.
If you fear this has happened you can delete the twitter file and use guys again to generate a new access token or simply block guys' access to your account from your twitter settings.

Remember, [physical access is root access](http://twitter.com/#search?q=twitter%20hijacked)

Instalation notes
------------

This application was made for ubuntu but should also work for other unix operating systems.

dependencies:
	* libcurl
	* libcrypto++
	* libuu 

to install:
	* [download the source](http://github.com/ramyD/guys/archives/master)
	* type "make"
	* move the "guys" executable to your /usr/local/bin/ or where ever


Release notes
------------

	* now uses Oauth
	* facebook dropped
	* identica progress halted
	* keys storred in local user's /config directory
	* does not store password locally, so config files can be deleted or permission revoked from the user's twitter page if necessary


