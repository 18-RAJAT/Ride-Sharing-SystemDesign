#include <bits/stdc++.h>
using namespace std;

class Rider
{
public:
    Rider(string& name,double latitude,double longitude):name(name),latitude(latitude),longitude(longitude){}

    string getName()const{return name;}
    double getLatitude()const{return latitude;}
    double getLongitude()const{return longitude;}
private:
    string name;
    double latitude;
    double longitude;
};

class Driver
{
public:
    Driver(string& name,double latitude,double longitude):name(name),latitude(latitude),longitude(longitude),available(true){}
    string getName()const{return name;}
    double getLatitude()const{return latitude;}
    double getLongitude()const{return longitude;}
    bool isAvailable()const{return available;}

    void setAvailability(bool status)
    {
        available=status;
    }
private:
    string name;
    double latitude;
    double longitude;
    bool available;
};

class RideMatchingService
{
public:
    void addDriver(const Driver& driver)
    {
        drivers.push_back(driver);
    }
    Driver* matchRider(const Rider& rider,double& fare)
    {
        Driver* nearestDriver=nullptr;
        double minDistance=numeric_limits<double>::max();
        for(auto& driver:drivers)
        {
            if(driver.isAvailable())
            {
                double distance=calculateDistance(rider,driver);
                if(distance<minDistance)
                {
                    minDistance=distance;
                    nearestDriver=&driver;
                }
            }       
        }
        if(nearestDriver)
        {
            nearestDriver->setAvailability(false);
            fare=calculateFare(minDistance);
            return nearestDriver;
        }
        return nullptr;
    }
private:
    vector<Driver> drivers;
    double calculateDistance(const Rider& rider,const Driver& driver)
    {
        double dx=rider.getLatitude()-driver.getLatitude();
        double dy=rider.getLongitude()-driver.getLongitude();
        return sqrt(dx*dx+dy*dy);
    }
    double calculateFare(double distance)
    {
        const double ratePerUnitDistance=5.0;
        return ratePerUnitDistance*distance;
    }
};

void getDriverInput(RideMatchingService& rideService)
{
    int numberOfDrivers;
    cout<<"Enter the number of drivers: ";
    cin>>numberOfDrivers;
    cin.ignore();
    for(int i=0;i<numberOfDrivers;++i)
    {
        string name;
        double latitude,longitude;
        cout<<"Enter name for Driver "<<(i+1)<<": ";
        getline(cin,name);
        cout<<"Enter latitude for Driver "<<(i+1)<<": ";
        cin>>latitude;
        cout<<"Enter longitude for Driver "<<(i+1)<<": ";   
        cin>>longitude;
        cin.ignore();
        rideService.addDriver(Driver(name,latitude,longitude));
    }   
}

Rider getRiderInput()
{
    string name;
    double latitude,longitude;
    cout<<"Enter rider's name: ";
    cin.ignore();
    getline(cin,name);
    cout<<"Enter rider's latitude: ";
    cin>>latitude;
    cout<<"Enter rider's longitude: ";
    cin>>longitude;
    return Rider(name,latitude,longitude);
}

void testRideMatching()
{
    RideMatchingService rideService;
    getDriverInput(rideService);
    Rider rider=getRiderInput();
    double fare;
    Driver* matchedDriver=rideService.matchRider(rider,fare);
    if(matchedDriver)
    {
        cout<<"Matched with "<<matchedDriver->getName()<<endl;
        cout<<"Fare: $"<<fare<<endl;
    }
    else
    {
        cout<<"No available drivers for "<<rider.getName()<<endl;
    }
}

int main()
{
    testRideMatching();
    return 0;
}