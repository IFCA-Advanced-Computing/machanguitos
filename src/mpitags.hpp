/*******************************************************************************
Machanguitos is The Easiest Multi-Agent System in the world. Work done at The
Institute of Physics of Cantabria (IFCA).
Copyright (C) 2013  Luis Cabellos

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
/** @file mpitags.hpp
    @brief Declaration of MPI message tags.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef MPITAGS_HPP
#define MPITAGS_HPP

//------------------------------------------------------------------------------
namespace Engine{
    /** Create Class Message Tag.
        @ingroup Engine
     */
    constexpr int TAG_CREATECLASS = 1;
    /** Create Agents Message Tag.
        @ingroup Engine
     */
    constexpr int TAG_CREATEAGENTS = 2;
    /** Run Agents Message Tag.
        @ingroup Engine
     */
    constexpr int TAG_RUNAGENTS = 3;
    /** Set Start Time Message Tag.
        @ingroup Engine
     */
    constexpr int TAG_SETSTARTTIME = 4;
    /** Set Data Store info.
        @ingroup Engine
     */
    constexpr int TAG_SETDATASTORE = 5;
    /** End Simulation Message Tag.
        @ingroup Engine
     */
    constexpr int TAG_END = 66;
}

//------------------------------------------------------------------------------
#endif//MPITAGS_HPP

//------------------------------------------------------------------------------
