# Ride-Sharing Architecture Overview

This document provides an overview of the architecture for a ride-sharing application, detailing the client-side applications, backend services, data storage, and service connections.

![Architecture Diagram](image/ride-sharing-system.png)

## Table of Contents
1. [Client-Side Applications](#client-side-applications)
2. [Backend Services](#backend-services)
3. [Data Storage and Databases](#data-storage-and-databases)
4. [Service Connections](#service-connections)
5. [Kafka Service](#kafka-service)
6. [Key Use Cases](#key-use-cases)

## Client-Side Applications

- **Rider App**
  - **Functionality**: Allows users to request rides, view drivers, track ride status, make payments, and rate drivers.

- **Driver App**
  - **Functionality**: Allows drivers to receive ride requests, navigate to the rider's location, manage ride status, and receive payments.

## Backend Services

- **API Gateway**
  - **Functionality**: Manages and routes requests from client-side applications to the appropriate backend services.

### Services
- **Ride Matching Service** (AWS EC2)
  - Matches riders with nearby drivers based on real-time location and availability.

- **Real-Time Location Tracking Service** (AWS EC2)
  - Tracks the real-time location of both riders and drivers during active rides and updates the system.

- **Pricing and ETA Service** (AWS EC2)
  - Calculates trip fare based on distance, time, traffic conditions, and surge pricing. Provides estimated time of arrival (ETA).

- **Notification Service** (AWS SNS)
  - Sends push notifications to both riders and drivers about ride requests, cancellations, trip start/end, etc.

- **Payment Service** (AWS EC2)
  - Handles payment transactions, including fare calculation, user billing, driver payout, and storing payment details.

- **Rating & Review Service** (AWS EC2)
  - Allows riders and drivers to leave ratings and reviews after the completion of a trip.

- **Trip Management Service** (AWS EC2)
  - Manages trip lifecycle, including ride request, acceptance, in-progress tracking, and completion.

- **User Management Service** (AWS Cognito)
  - Handles user authentication and authorization, including managing rider and driver profiles.

- **Surge Pricing & Demand Prediction** (AWS EC2)
  - Analyzes demand patterns in different areas to adjust prices during high-demand periods (surge pricing).

- **Kafka Service**
  - Manages real-time event streams for efficient message exchange between services.

## Data Storage and Databases

- **User Database** (PostgreSQL)
  - Stores user data such as profiles, authentication information, and payment details.

- **Ride Database** (MongoDB)
  - Stores ride-related data, including trip history, driver/rider interactions, and location data.

- **Cache** (Redis)
  - Caches frequently accessed data (such as driver locations and pricing details) to reduce latency and improve system performance.

- **Data Warehouse** (AWS Redshift)
  - Stores and processes large amounts of historical data for analytics, demand prediction, and reporting.

- **Blob Storage** (AWS S3)
  - Stores large unstructured data like trip receipts, driver/rider images, and documents.

## Service Connections

- Rider App → API Gateway
- Driver App → API Gateway

- API Gateway → Ride Matching Service
- API Gateway → Real-Time Location Tracking Service
- API Gateway → Pricing and ETA Service
- API Gateway → Notification Service
- API Gateway → Payment Service
- API Gateway → Rating & Review Service
- API Gateway → Trip Management Service
- API Gateway → User Management Service
- API Gateway → Surge Pricing & Demand Prediction

- Ride Matching Service ↔ Ride Database
- Real-Time Location Tracking Service ↔ Ride Database
- Pricing and ETA Service ↔ Cache
- Payment Service ↔ User Database
- Rating & Review Service ↔ User Database
- Trip Management Service ↔ User Database
- User Management Service ↔ User Database
- Surge Pricing & Demand Prediction ↔ Data Warehouse
- Blob Storage → User Database: Store receipts

## Kafka Service

The Kafka Service is responsible for:

- **Real-Time Event Streaming**: Facilitates communication between microservices by streaming events in real-time.
- **Decoupling Services**: Enables asynchronous communication, allowing services to function independently.
- **Handling Event-Driven Workflows**: Captures events like ride requests, trip updates, and notifications for processing by different services.
- **Event Persistence and Replay**: Retains event logs for a configurable period, allowing services to recover missed events.

## Key Use Cases

- **Ride Request Processing**
  - Event: "ride_request"
  - Publisher: Rider App
  - Consumer: Ride Matching Service
  - Matches riders with drivers based on incoming ride requests.

- **Real-Time Location Updates**
  - Event: "location_update"
  - Publisher: Real-Time Location Tracking Service
  - Consumers: Trip Management Service, Rider App, Driver App
  - Provides real-time updates on rider and driver locations.

- **Notification Handling**
  - Event: "trip_status_change"
  - Publisher: Trip Management Service
  - Consumer: Notification Service
  - Sends notifications to users about trip status changes.

- **Trip Lifecycle Management**
  - Events: "ride_accepted", "trip_started", "trip_completed"
  - Publisher: Ride Matching Service, Trip Management Service
  - Consumer: Payment Service
  - Handles trip lifecycle and billing based on completed trips.

## Service-to-Kafka Connections

- Ride Matching Service ↔ Kafka
  - Publishes and subscribes to ride-related events (e.g., ride requests, cancellations).

- Real-Time Location Tracking Service ↔ Kafka
  - Publishes location updates for riders and drivers.

- Notification Service ↔ Kafka
  - Listens for events to send real-time notifications.

- Trip Management Service ↔ Kafka
  - Manages trip lifecycle based on ride-related events.

- Payment Service ↔ Kafka
  - Subscribes to trip completion events for fare calculation and driver payout.
