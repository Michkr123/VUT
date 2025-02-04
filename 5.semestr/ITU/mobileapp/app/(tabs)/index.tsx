// ITU Projekt
// Mobilní aplikace - Akce pro lidi
// index.tsx - Přehled všech dostupných akcí
// Autor: Marek Antoňů (xanton07)

import { BACKEND_IP, BACKEND_PORT } from '@/backend_address';
import { useUser } from '@/context/UserContext';
import type { Event, EventCategory, Region } from '@/types/Event';
import { useFocusEffect } from '@react-navigation/native';
import { router } from 'expo-router';
import React, { useEffect, useState } from 'react';
import {
  ActivityIndicator,
  FlatList,
  Image,
  ScrollView,
  StyleSheet,
  Text,
  TouchableOpacity,
  View
} from 'react-native';
import { Rating } from 'react-native-ratings';

// Definice kategorií událostí s jejich ikonami a popisky
const EVENT_BUTTONS = [
  { value: 'entertainment' as EventCategory, label: 'Zábava', emoji: '🎭' },
  { value: 'education' as EventCategory, label: 'Vzdělání', emoji: '📚' },
  { value: 'for_adults' as EventCategory, label: 'Pro dospělé', emoji: '🍷' },
  { value: 'for_children' as EventCategory, label: 'Pro děti', emoji: '🧸' },
  { value: 'workshop' as EventCategory, label: 'Workshop', emoji: '🛠️' },
  { value: 'exposition' as EventCategory, label: 'Výstava', emoji: '🖼️' },
];

// Seznam krajů pro filtrování
const REGIONS = [
  { value: 'praha' as Region, label: 'Praha' },
  { value: 'stredocesky' as Region, label: 'Středočeský' },
  { value: 'jihocesky' as Region, label: 'Jihočeský' },
  { value: 'plzensky' as Region, label: 'Plzeňský' },
  { value: 'karlovarsky' as Region, label: 'Karlovarský' },
  { value: 'ustecky' as Region, label: 'Ústecký' },
  { value: 'liberecky' as Region, label: 'Liberecký' },
  { value: 'kralovehradecky' as Region, label: 'Královéhradecký' },
  { value: 'pardubicky' as Region, label: 'Pardubický' },
  { value: 'vysocina' as Region, label: 'Vysočina' },
  { value: 'jihomoravsky' as Region, label: 'Jihomoravský' },
  { value: 'olomoucky' as Region, label: 'Olomoucký' },
  { value: 'moravskoslezsky' as Region, label: 'Moravskoslezský' },
  { value: 'zlinsky' as Region, label: 'Zlínský' },
];

// Typ položky v seznamu (header nebo událost)
type ListItem = 
  | { type: 'header' }
  | { type: 'event'; data: Event }

export default function EventsTab() {
  const { login } = useUser();
  const [events, setEvents] = useState<Event[]>([]);
  const [filteredEvents, setFilteredEvents] = useState<Event[]>([]);
  const [region, setRegion] = useState<Region>('');
  const [category, setCategory] = useState<EventCategory | ''>('');
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [needsRefresh, setNeedsRefresh] = useState(false);

  // Úvodní načtení dat
  useEffect(() => {
    fetchEvents();
  }, []);

  // Obnovení při návratu na stránku
  useFocusEffect(
    React.useCallback(() => {
      if (needsRefresh) {
        fetchEvents();
        setNeedsRefresh(false);
      }
    }, [needsRefresh])
  );

  // Vytvoření nové události
  const handleAddEvent = () => {
    setNeedsRefresh(true);
    router.push('/(events)/create');
  };

  // Vykreslení hlavičky s filtry
  const renderHeader = () => (
    <View>
      <View style={styles.header}>
        <Text style={styles.headerTitle}>Filtrování podle kategorie a krajů</Text>
        {login === 'admin' && (
          <TouchableOpacity 
            style={styles.addButton}
            onPress={handleAddEvent}
          >
            <Text style={styles.addButtonText}>+ Přidat akci</Text>
          </TouchableOpacity>
        )}
      </View>
      
      {/* Filtry */}
      <View style={styles.filtersContainer}>

        {/* Seznam krajů */}
        <ScrollView 
          horizontal 
          showsHorizontalScrollIndicator={false}
          style={styles.regionScroll}
        >
          <TouchableOpacity
            style={[styles.regionButton, !region && styles.activeRegionButton]}
            onPress={() => setRegion('')}
          >
            <Text style={[styles.regionText, !region && styles.activeRegionText]}>
              Všechny regiony
            </Text>
          </TouchableOpacity>
          {REGIONS.map((reg) => (
            <TouchableOpacity
              key={reg.value}
              style={[
                styles.regionButton,
                region === reg.value && styles.activeRegionButton
              ]}
              onPress={() => setRegion(reg.value)}
            >
              <Text style={[
                styles.regionText,
                region === reg.value && styles.activeRegionText
              ]}>
                {reg.label}
              </Text>
            </TouchableOpacity>
          ))}
        </ScrollView>
        
        {/* Seznam kategorií */}
        <ScrollView 
          horizontal 
          showsHorizontalScrollIndicator={false}
          style={styles.categoryScroll}
        >
          {EVENT_BUTTONS.map((btn) => (
            <TouchableOpacity
              key={btn.value}
              style={[
                styles.categoryButton,
                category === btn.value && styles.activeCategoryButton
              ]}
              onPress={() => setCategory(category === btn.value ? '' : btn.value)}
            >
              <Text style={styles.categoryEmoji}>{btn.emoji}</Text>
              <Text style={[
                styles.categoryText,
                category === btn.value && styles.activeCategoryText
              ]}>
                {btn.label}
              </Text>
            </TouchableOpacity>
          ))}
        </ScrollView>
      </View>
    </View>
  );

  // Filtrování událostí podle vybraných filtrů
  useEffect(() => {
    if (!events) return;

    const filtered = events.filter((event) => {
      const matchesRegion = !region || event.region === region;
      const matchesCategory = !category || event.event_category === category;
      return matchesRegion && matchesCategory;
    });

    setFilteredEvents(filtered);
  }, [events, region, category]);

  // Načtení událostí
  const fetchEvents = async () => {
    try {
      setIsLoading(true);
      setError(null);
      const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/events`);
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();
      setEvents(data);
      setFilteredEvents(data);
    } catch (error) {
      console.error("Error fetching events:", error);
      setError("Nepodařilo se načíst události. Zkuste to prosím později.");
    } finally {
      setIsLoading(false);
    }
  };

  // Formátování data pro zobrazení
  const formatDate = (dateString: string) => {
    return new Date(dateString).toLocaleDateString('cs-CZ', {
      day: 'numeric',
      month: 'long',
      year: 'numeric'
    });
  };

  // Přesměrování na detail události
  const handleEventPress = (eventId: number) => {
    setNeedsRefresh(true);
    router.push(`/(events)/${eventId}`);
  };

  // Přesměrování na detail události
  const listData: ListItem[] = [
    { type: 'header' },
    ...filteredEvents.map((event): ListItem => ({ type: 'event', data: event }))
  ];

  // Vykreslení položky seznamu
  const renderItem = ({ item }: { item: ListItem }) => {
    if (item.type === 'header') {
      return renderHeader();
    }
    return <EventCard item={item.data} />;
  };

  // Komponenta pro kartu události
  const EventCard = ({ item }: { item: Event }) => {
    const [imageUri, setImageUri] = useState<string | null>(null);
    const [imageLoading, setImageLoading] = useState(false);

    // Načtení obrázku události
    useEffect(() => {
      const fetchImage = async () => {
        if (item.image) {
          setImageLoading(true);
          try {
            const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/events/${item.id}/image`);
            if (response.ok) {
              const data = await response.json();
              setImageUri(data.image);
            }
          } catch (error) {
            console.error('Error fetching event image:', error);
          } finally {
            setImageLoading(false);
          }
        }
      };

      fetchImage();
    }, [item.id]);

    return (
      <TouchableOpacity 
        style={styles.eventCard}
        onPress={() => handleEventPress(item.id)}
      >
        
        {imageLoading ? (
          <View style={[styles.eventImage, styles.imagePlaceholder]}>
            <ActivityIndicator size="large" color="#2196f3" />
          </View>
        ) : imageUri && (
          <Image
            source={{ uri: imageUri }}
            style={styles.eventImage}
            resizeMode="cover"
          />
        )}

        <View style={styles.eventHeader}>
          <Text style={styles.eventIcon}>
            {EVENT_BUTTONS.find(btn => btn.value === item.event_category)?.emoji || '📅'}
          </Text>
          <Text style={styles.eventName}>{item.name}</Text>
        </View>
        
        <View style={styles.eventDetails}>
          <Text style={styles.eventDate}>{formatDate(item.date_of_event)}</Text>
          <Text style={styles.eventLocation}>📍 {item.city}, {item.address}</Text>
          {item.average_rating && (
          <View style={styles.ratingContainer}>
            <Rating
              type="custom"
              ratingCount={5}
              startingValue={item.average_rating}
              imageSize={16}
              readonly={true}
              fractions={2}
              tintColor="#fff"
              ratingBackgroundColor="#ddd"
              style={{ padding: 5 }}
            />
            {item.review_count && (
              <Text style={styles.reviewCount}>
                ({item.review_count})
              </Text>
            )}
          </View>
        )}
        </View>
      </TouchableOpacity>
    );
  };

  if (error) {
    return (
      <View style={styles.centerContainer}>
        <Text style={styles.errorText}>{error}</Text>
      </View>
    );
  }

  // Hlavní render komponenty
  return (
    <View style={styles.container}>
      {isLoading ? (
        <View style={styles.centerContainer}>
          <ActivityIndicator size="large" color="#2196f3" />
        </View>
      ) : (
        <FlatList
          data={listData}
          renderItem={renderItem}
          keyExtractor={(item) => 
            item.type === 'header' ? 'header' : item.data.id.toString()
          }
          contentContainerStyle={styles.listContainer}
        />
      )}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f5f5',
  },
  centerContainer: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  header: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    padding: 20,
    backgroundColor: '#fff',
    borderBottomWidth: 1,
    borderBottomColor: '#e0e0e0',
  },
  headerTitle: {
    fontSize: 16,
    fontWeight: 'bold',
  },
  addButton: {
    backgroundColor: '#2196f3',
    paddingHorizontal: 15,
    paddingVertical: 8,
    borderRadius: 20,
    marginHorizontal: 15,
    marginVertical: 10,
  },
  addButtonText: {
    color: '#fff',
    fontWeight: '500',
  },
  eventImage: {
    width: '100%',
    height: 150,
    borderTopLeftRadius: 10,
    borderTopRightRadius: 10,
    marginBottom: 10,
  },
  filtersContainer: {
    backgroundColor: '#fff',
    paddingVertical: 10,
  },
  regionScroll: {
    paddingHorizontal: 15,
    marginBottom: 10,
  },
  categoryScroll: {
    paddingHorizontal: 15,
  },
  regionButton: {
    paddingHorizontal: 15,
    paddingVertical: 8,
    borderRadius: 20,
    backgroundColor: '#f0f0f0',
    marginRight: 8,
  },
  activeRegionButton: {
    backgroundColor: '#2196f3',
  },
  regionText: {
    color: '#666',
  },
  activeRegionText: {
    color: '#fff',
  },
  categoryButton: {
    flexDirection: 'row',
    alignItems: 'center',
    paddingHorizontal: 15,
    paddingVertical: 8,
    borderRadius: 20,
    backgroundColor: '#f0f0f0',
    marginRight: 8,
  },
  activeCategoryButton: {
    backgroundColor: '#2196f3',
  },
  categoryEmoji: {
    marginRight: 5,
  },
  categoryText: {
    color: '#666',
  },
  activeCategoryText: {
    color: '#fff',
  },
  errorText: {
    color: '#f44336',
    textAlign: 'center',
    margin: 20,
  },
  listContainer: {
    padding: 10,
  },
  eventCard: {
    backgroundColor: '#fff',
    borderRadius: 10,
    padding: 15,
    marginBottom: 10,
    shadowColor: '#000',
    shadowOffset: {
      width: 0,
      height: 2,
    },
    shadowOpacity: 0.1,
    shadowRadius: 3,
    elevation: 3,
  },
  eventHeader: {
    flexDirection: 'row',
    alignItems: 'center',
    marginBottom: 10,
  },
  eventIcon: {
    fontSize: 24,
    marginRight: 10,
  },
  eventName: {
    fontSize: 18,
    fontWeight: '600',
    flex: 1,
  },
  eventDetails: {
    paddingLeft: 34,
  },
  eventDate: {
    fontSize: 16,
    color: '#666',
    marginBottom: 5,
  },
  eventLocation: {
    fontSize: 14,
    color: '#666',
    marginBottom: 5,
  },
  ratingContainer: {
    flexDirection: 'row',
    alignItems: 'center',
    marginTop: 5,
  },
  reviewCount: {
    fontSize: 14,
    color: '#666',
    marginLeft: 5,
  },
  rating: {
    fontSize: 16,
  },
  imagePlaceholder: {
    backgroundColor: '#f0f0f0',
    justifyContent: 'center',
    alignItems: 'center',
  },
});
