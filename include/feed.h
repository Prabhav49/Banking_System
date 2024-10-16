#ifndef FEED_H
#define FEED_H

#include <stdio.h>
#include <string.h>
#include <time.h>

#define FEEDBACK_DB_PATH "../db/feed.db"  

typedef struct {
    int feedNo;                   // Unique feedback number
    char username[30];            // Username of the person giving feedback
    char feedback[300];           // Feedback message (2-3 sentences)
    char status[15];              // Status: "Not Seen", "Working", "Resolved"
    char feedbackTime[50];        // Time when feedback was added
    char statusUpdateTime[50];    // Time when the status was updated (N/A initially)
} Feedback;

// Function to add feedback from a customer
void addFeedback(const char *username);

#endif // FEED_H