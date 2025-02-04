// ITU Projekt
// Mobilní aplikace - Akce pro lidi
// calendar.tsx - Kalendář akcí zaregistrovaných uživatelem
// Autor: Petr Hron (xhronpe00)

import { BACKEND_IP, BACKEND_PORT } from '@/backend_address';
import { useUser } from '@/context/UserContext';
import { Event } from '@/types/Event';
import { useFocusEffect } from '@react-navigation/native';
import { router } from 'expo-router';
import React, { useEffect, useState } from 'react';
import { ScrollView, StyleSheet, Text, TouchableOpacity, View } from 'react-native';
import { Calendar, DateData } from 'react-native-calendars';

// Typ pro označené datumy v kalendáři
interface MarkedDates {
  [date: string]: {
    marked: boolean;
    selected?: boolean;
    selectedColor?: string;
    dotColor?: string;
  };
}

export default function CalendarTab() {
  const [events, setEvents] = useState<Event[]>([]);
  const [selectedDate, setSelectedDate] = useState('');
  const [markedDates, setMarkedDates] = useState<MarkedDates>({});
  const { login } = useUser();
  const [profile, setProfile] = useState<any>(null);

  // Obnovení dat při návratu na obrazovku
  useFocusEffect(
    React.useCallback(() => {
      const fetchData = async () => {
        try {
          // Načtení profilu a jeho akcí
          const profileResponse = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/profiles/${login}`);
          const profileData = await profileResponse.json();
          setProfile(profileData);

          // Načtení všech událostí
          const eventsResponse = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/events`);
          const allEvents = await eventsResponse.json();
          
          // Filtrování událostí podle registrací uživatele
          const userEvents = allEvents.filter((event: Event) => 
            profileData.visitorActions.includes(event.id) || 
            profileData.workerActions.includes(event.id)
          );
          
          // Seřazení událostí podle data
          const sortedEvents = userEvents.sort((a: Event, b: Event) => 
            new Date(a.date_of_event).getTime() - new Date(b.date_of_event).getTime()
          );
          
          setEvents(sortedEvents);

          // Označení dnů s událostmi v kalendáři
          const marked: MarkedDates = {};
          userEvents.forEach((event: Event) => {
            const date = event.date_of_event.split('T')[0];
            const isVisitor = profileData.visitorActions.includes(event.id);
            marked[date] = {
              marked: true,
              selectedColor: isVisitor ? '#2196f3' : '#4CAF50', // Modrá pro návštěvníky, zelená pro dobrovolníky
              dotColor: isVisitor ? '#2196f3' : '#4CAF50'
            };
          });
          setMarkedDates(marked);
        } catch (error) {
          console.error('Error fetching data:', error);
        }
      };

      fetchData();
    }, [login])
  );

  // Aktualizace událostí při změně profilu
  useEffect(() => {
    const fetchEvents = async () => {
      try {
        const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/events`);
        const allEvents = await response.json();
        
        if (profile) {
          // Filtrování událostí pro přihlášeného uživatele
          const userEvents = allEvents.filter((event: Event) => 
            profile.visitorActions.includes(event.id) || 
            profile.workerActions.includes(event.id)
          );
          
          const sortedEvents = userEvents.sort((a: Event, b: Event) => 
            new Date(a.date_of_event).getTime() - new Date(b.date_of_event).getTime()
          );
          
          setEvents(sortedEvents);

          // Aktualizace označených dnů v kalendáři
          const marked: MarkedDates = {};
          userEvents.forEach((event: Event) => {
            const date = event.date_of_event.split('T')[0];
            const isVisitor = profile.visitorActions.includes(event.id);
            marked[date] = {
              marked: true,
              selectedColor: isVisitor ? '#2196f3' : '#4CAF50',
              dotColor: isVisitor ? '#2196f3' : '#4CAF50'
            };
          });
          setMarkedDates(marked);
        }
      } catch (error) {
        console.error('Error fetching events:', error);
      }
    };

    if (profile) {
      fetchEvents();
    }
  }, [profile]);

  // Získání událostí pro vybraný den
  const getEventsForDate = (date: string) => {
    return events.filter(event => event.date_of_event.startsWith(date));
  };

  // Přesměrování na detail události
  const handleEventPress = (eventId: number) => {
    router.push(`/(events)/${eventId}`);
  };

  // Vykreslení karty události
  const renderEventCard = (event: Event) => {
    const isVisitor = profile?.visitorActions.includes(event.id);
    
    return (
      <TouchableOpacity 
        key={event.id}
        style={[styles.eventCard, {
          borderLeftColor: isVisitor ? '#2196f3' : '#4CAF50',
          borderLeftWidth: 5,
        }]}
        onPress={() => handleEventPress(event.id)}
      >
        <View>
          <Text style={styles.eventName}>{event.name}</Text>
          <Text style={styles.eventLocation}>📍 {event.city}, {event.address}</Text>
          <Text style={styles.eventRole}>
            Role: {isVisitor ? 'Návštěvník' : 'Dobrovolník'}
          </Text>
        </View>
      </TouchableOpacity>
    );
  };

  return (
    <View style={styles.container}>
      {/* Kalendář s označenými dny */}
      <Calendar
        onDayPress={(day: DateData) => setSelectedDate(day.dateString)}
        markedDates={{
          ...markedDates,
          [selectedDate]: {
            ...markedDates[selectedDate],
            selected: true,
          }
        }}
        theme={{
          selectedDayBackgroundColor: '#2196f3',
          todayTextColor: '#2196f3',
          arrowColor: '#2196f3',
        }}
      />

      {/* Seznam událostí pro vybraný den */}
      {selectedDate && getEventsForDate(selectedDate).length > 0 && (
        <View style={styles.eventList}>
          <Text style={styles.dateHeader}>
            Události {new Date(selectedDate).toLocaleDateString('cs-CZ')}
          </Text>
          <ScrollView>
            {getEventsForDate(selectedDate).map(renderEventCard)}
          </ScrollView>
        </View>
      )}
    </View>
  );
}

// Styly komponent
const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f5f5',
  },
  eventList: {
    padding: 15,
    flex: 1,
  },
  dateHeader: {
    fontSize: 18,
    fontWeight: '600',
    marginBottom: 10,
    color: '#333',
  },
  eventCard: {
    backgroundColor: '#fff',
    borderRadius: 8,
    padding: 15,
    marginBottom: 10,
    elevation: 2,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 2,
  },
  eventName: {
    fontSize: 16,
    fontWeight: '600',
    marginBottom: 5,
  },
  eventLocation: {
    fontSize: 14,
    color: '#666',
    marginBottom: 5,
  },
  eventRole: {
    fontSize: 14,
    fontWeight: '500',
    color: '#2196f3',
  },
});
