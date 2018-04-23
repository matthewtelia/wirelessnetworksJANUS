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

        @display("bgb=706,452");
    submodules:

        SensorNode1: SensorNode {
            @display("p=521,332");
        }
        SensorNode2: SensorNode {
            @display("p=169,116");
        }
        AP: AP {
		//WirelessChannel: WirelessChannel;

            @display("p=521,52");
        }
    connections:

        AP.out[0] --> SensorNode1.in++;
        AP.out[1] --> SensorNode2.in++;
        SensorNode1.out[0] --> AP.in++;
        SensorNode1.out[1] --> SensorNode2.in++;
        SensorNode2.out[0] --> AP.in++;
        SensorNode2.out[1] --> SensorNode1.in++;
}
