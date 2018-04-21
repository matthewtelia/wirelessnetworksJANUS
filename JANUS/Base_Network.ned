//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

network Base_Network
{
	parameters:
	    
	submodules:
	
		SensorNode1: SensorNode;
		SensorNode2: SensorNode;
		AP: AP;
		//WirelessChannel: WirelessChannel;
		
	connections:
	            
	    AP.out1 --> SensorNode1.in;
	    AP.out2 --> SensorNode2.in;
	    SensorNode1.out1 --> AP.in;
	    SensorNode1.out2 --> SensorNode2.in;
	    SensorNode2.out1 --> AP.in;
	    SensorNode2.out2 --> SensorNode1.in;
}