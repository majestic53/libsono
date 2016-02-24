# libsono
# Copyright (C) 2016 David Jolly
# ----------------------
#
# libsono is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libsono is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

from distutils.core import setup, Extension

# Module information

setup(
	name='sonopkg', 
	version='0.1', 
	description='a lightwight library used to control Sono devices', 
	author='David Jolly',
	author_email='majestic53@gmail.com',
	url='https://github.com/majestic53/libsono',
	ext_modules=[Extension('sono_py', sources=['./src/sono_py.c'])],
	)
