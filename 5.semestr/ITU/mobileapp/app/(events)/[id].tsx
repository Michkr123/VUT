// ITU Projekt
// Mobilní aplikace - Akce pro lidi
// (events)/[id].tsx - Detail jednotlivých akcí podle ID
// Autor: Marek Antoňů (xanton07)

import React, { useState, useEffect } from 'react';
import { useFocusEffect } from '@react-navigation/native';
import { Rating } from 'react-native-ratings';
import { 
  View, 
  Text, 
  Image,
  StyleSheet, 
  ScrollView, 
  ActivityIndicator,
  TouchableOpacity,
  Alert,
  RefreshControl
} from 'react-native';
import { useLocalSearchParams, router } from 'expo-router';
import { BACKEND_IP, BACKEND_PORT } from '@/backend_address';
import { useUser } from '@/context/UserContext';
import { Profile } from '@/types/Profile';
import { Event } from '@/types/Event';

// Funkce pro zjištění stavu přihlášení na akci
const getRegistrationStatus = (profile: Profile | null, eventId: number) => {
  if (!profile) return 'none';
  
  if (profile.visitorActions.includes(eventId)) return 'visitor';
  if (profile.workerActions.includes(eventId)) return 'worker';
  
  return 'none';
}

export default function EventDetails() {
  const { id } = useLocalSearchParams();
  const { login } = useUser();
  const [event, setEvent] = useState<Event | null>(null);
  const [eventImage, setEventImage] = useState<string | null>(null);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [isRefreshing, setIsRefreshing] = useState(false);
  const [needsRefresh, setNeedsRefresh] = useState(false);
  const [userProfile, setUserProfile] = useState<Profile | null>(null);
  const [imageLoading, setImageLoading] = useState(false);
  const [isRegistering, setIsRegistering] = useState(false);

  const registrationStatus = userProfile ? getRegistrationStatus(userProfile, Number(id)) : 'none';

  // Úvodní refresh
  useEffect(() => {
    fetchEventDetails();
  }, [id]);

  // Získání dat přihlášeného uživatele
  useEffect(() => {
    const fetchUserProfile = async () => {
      try {
        const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/profiles/${login}`);
        if (response.ok) {
          const data = await response.json();
          setUserProfile(data);
        }
      } catch (error) {
        console.error('Error fetching profile:', error);
      }
    };

    fetchUserProfile();
  }, [login]);

  // Získání obrázku akce
  useEffect(() => {
    const fetchImage = async () => {
      // if (event?.image) {
        setImageLoading(true);
        try {
          const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/events/${id}/image`);
          if (response.ok) {
            const data = await response.json();
            setEventImage(data.image);
          }
        } catch (error) {
          console.error('Error fetching event image:', error);
        } finally {
          setImageLoading(false);
        }
      // }
    };

    if (event) {
      fetchImage();
    }
  }, [event]);

  // Refresh když se vracíme zpět na zobrazení přehledu
  useFocusEffect(
    React.useCallback(() => {
      if (needsRefresh) { // Refresh pouze pokud mame data
        refreshEventDetails();
        setNeedsRefresh(false);
      }
    }, [id, needsRefresh])
  );

  // Registrace jako návštěvník nebo dobrovolník
  const handleRegistration = async (role: 'visitor' | 'worker') => {
    if (!userProfile) return;

    const isRegistered = registrationStatus !== 'none';
    const currentRole = registrationStatus;
    
    try {
      setIsRegistering(true);

      // Pokud už uživatel je registrovaný odregistruj
      if (isRegistered) {
        const unregisterResponse = await fetch(
          `http://${BACKEND_IP}:${BACKEND_PORT}/events/${id}/unregister`,
          {
            method: 'POST',
            headers: {
              'Content-Type': 'application/json',
            },
            body: JSON.stringify({ login, role: currentRole }),
          }
        );
        
        if (!unregisterResponse.ok) {
          throw new Error('Failed to unregister');
        }

        // Pokud je zvolena stejná role uživatel je odregistrovaný
        if (currentRole === role) {
          const updatedProfile = await unregisterResponse.json();
          setUserProfile(updatedProfile);
          Alert.alert('Úspěch', 'Odregistrace byla úspěšná');
          return;
        }
      }

      // Registrace nové role
      const registerResponse = await fetch(
        `http://${BACKEND_IP}:${BACKEND_PORT}/events/${id}/register`,
        {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({ login, role }),
        }
      );

      if (registerResponse.ok) {
        const updatedProfile = await registerResponse.json();
        setUserProfile(updatedProfile);
        Alert.alert('Úspěch', 'Registrace byla úspěšná');
      } else {
        throw new Error('Failed to register');
      }
    } catch (error) {
      console.error('Error updating registration:', error);
      Alert.alert('Chyba', 'Nepodařilo se aktualizovat registraci');
    } finally {
      setIsRegistering(false);
    }
  };

  // Přesměrování na úpravu události
  const handleEditEvent = () => {
    setNeedsRefresh(true);
    router.push(`/(events)/edit/${id}`);
  }

  // Získání dat o události z backendu
  const fetchEventDetails = async () => {
    try {
      setIsLoading(true);
      setError(null);
      const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/events/${id}`);
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();
      setEvent(data);
    } catch (error) {
      console.error("Error fetching event details:", error);
      setError("Nepodařilo se načíst detail události.");
    } finally {
      setIsLoading(false);
    }
  };

  // Formátování data
  const formatDate = (dateString: string) => {
    return new Date(dateString).toLocaleDateString('cs-CZ', {
      day: 'numeric',
      month: 'long',
      year: 'numeric'
    });
  };

  // Formátování data a času
  const formatDateTime = (dateString: string) => {
    return new Date(dateString).toLocaleDateString('cs-CZ', {
      day: 'numeric',
      month: 'long',
      year: 'numeric',
      hour: '2-digit',
      minute: '2-digit'
    });
  };
  
  // Změna prvního písmena na velké
  const capitalizeFirstLetter = (str: string) => {
    return str.charAt(0).toUpperCase() + str.slice(1);
  }

  // Přesměrování na vytvoření nové recenze
  const handleAddReview = () => {
    setNeedsRefresh(true);
    router.push(`/(events)/review?eventId=${id}`);
  };

  // Přesměrování na úpravu recenze
  const handleEditReview = (reviewId: number) => {
    setNeedsRefresh(true);
    router.push(`/(events)/review?eventId=${id}&reviewId=${reviewId}`);
  };

  // Like recenze
  const handleLikeReview = async (reviewId: number) => {
    try {
      const response = await fetch(
        `http://${BACKEND_IP}:${BACKEND_PORT}/events/${id}/reviews/${reviewId}/like`,
        {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({ login }),
        }
      );
      if (response.ok) {
        const updatedReview = await response.json();
        setEvent(prevEvent => {
          if (!prevEvent) return null;
          return {
            ...prevEvent,
            reviews: prevEvent.reviews.map(review =>
              review.id === reviewId ? updatedReview : review
            )
          };
        });
      }
    } catch (error) {
      console.error('Error liking review:', error);
      Alert.alert('Chyba', 'Nepodařilo se přidat like.');
    }
  };

  // Disike recenze
  const handleDislikeReview = async (reviewId: number) => {
    try {
      const response = await fetch(
        `http://${BACKEND_IP}:${BACKEND_PORT}/events/${id}/reviews/${reviewId}/dislike`,
        {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({ login }),
        }
      );
      if (response.ok) {
        const updatedReview = await response.json();
        setEvent(prevEvent => {
          if (!prevEvent) return null;
          return {
            ...prevEvent,
            reviews: prevEvent.reviews.map(review =>
              review.id === reviewId ? updatedReview : review
            )
          };
        });
      }
    } catch (error) {
      console.error('Error disliking review:', error);
      Alert.alert('Chyba', 'Nepodařilo se přidat dislike.');
    }
  };

  // Refreshnutí detailů akce
  const refreshEventDetails = async () => {
    try {
      setIsRefreshing(true);
      const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/events/${id}`);
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();
      setEvent(data);
    } catch (error) {
      console.error("Error refreshing event details:", error);
      Alert.alert('Chyba', 'Nepodařilo se aktualizovat data.');
    } finally {
      setIsRefreshing(false);
    }
  };

  // Odstranění akce
  const handleDeleteEvent = () => {
    Alert.alert(
      "Smazat událost",
      "Opravdu chcete smazat tuto událost?",
      [
        {
          text: "Zrušit",
          style: "cancel"
        },
        {
          text: "Smazat",
          style: "destructive",
          onPress: async () => {
            try {
              const response = await fetch(
                `http://${BACKEND_IP}:${BACKEND_PORT}/events/${id}`,
                { method: 'DELETE' }
              );
              if (response.ok) {
                router.back();
              } else {
                Alert.alert('Chyba', 'Nepodařilo se smazat událost');
              }
            } catch (error) {
              console.error('Error deleting event:', error);
              Alert.alert('Chyba', 'Nepodařilo se smazat událost');
            }
          }
        }
      ]
    );
  };

  // Odstranění recenze
  const handleDeleteReview = async (reviewId: number) => {
    Alert.alert(
      "Smazat recenzi",
      "Opravdu chcete smazat tuto recenzi?",
      [
        {
          text: "Zrušit",
          style: "cancel"
        },
        {
          text: "Smazat",
          style: "destructive",
          onPress: async () => {
            try {
              const response = await fetch(
                `http://${BACKEND_IP}:${BACKEND_PORT}/events/${id}/reviews/${reviewId}`,
                {
                  method: 'DELETE',
                }
              );
              if (response.ok) {
                setEvent(prevEvent => {
                  if (!prevEvent) return null;
                  // Vyfiltrovat odstraněné recenze
                  const updatedReviews = prevEvent.reviews.filter(review => review.id !== reviewId);
                  // Vypočítat nový průměr hodnocení
                  const newAverageRating = updatedReviews.length > 0
                    ? updatedReviews.reduce((sum, review) => sum + review.rating, 0) / updatedReviews.length
                    : null;
                  
                  return {
                    ...prevEvent,
                    reviews: updatedReviews,
                    average_rating: newAverageRating,
                    review_count: updatedReviews.length
                  };
                });
              }
            } catch (error) {
              console.error('Error deleting review:', error);
              Alert.alert('Chyba', 'Nepodařilo se smazat recenzi.');
            }
          }
        }
      ]
    );
  };

  // Zobrazit aktivitu při načítání
  if (isLoading) {
    return (
      <View style={styles.centerContainer}>
        <ActivityIndicator size="large" color="#2196f3" />
      </View>
    );
  }

  // Zobrazit chybovou hlášku
  if (error || !event) {
    return (
      <View style={styles.centerContainer}>
        <Text style={styles.errorText}>{error}</Text>
      </View>
    );
  }
  
  return (
    <ScrollView
      style={styles.container}
      refreshControl={
        <RefreshControl
          refreshing={isRefreshing}
          onRefresh={refreshEventDetails}
        />
      }
    >

      {/* Zobrazení tlačítek "Upravit" a "Smazat" pro admina */}
      {login === 'admin' && (
        <View style={styles.adminActions}>
          <TouchableOpacity
            style={[styles.adminButton, styles.editButton]}
            onPress={handleEditEvent}
          >
            <Text style={styles.adminButtonText}>Upravit</Text>
          </TouchableOpacity>
          <TouchableOpacity
            style={[styles.adminButton, styles.deleteButton]}
            onPress={handleDeleteEvent}
          >
            <Text style={styles.adminButtonText}>Smazat</Text>
          </TouchableOpacity>
        </View>
      )}

      {/* Obrázek akce */}
      {imageLoading ? (
        <View style={styles.imageContainer}>
          <ActivityIndicator size="large" color="#2196f3" />
        </View>
      ) : eventImage && (
        <View style={styles.imageContainer}>
          <Image
            source={{ uri: eventImage }}
            style={styles.eventImage}
            resizeMode="cover"
          />
        </View>
      )}

      {/* Hlavička akce s hodnocením */}
      <View style={styles.header}>
        <Text style={styles.title}>{event.name}</Text>
        {event.average_rating && (
          <View style={styles.ratingContainer}>
            <Rating
              type="custom"
              ratingCount={5}
              startingValue={event.average_rating}
              imageSize={24}
              readonly={true}
              fractions={2}
              tintColor="#fff"
              ratingBackgroundColor="#ddd"
              style={{ paddingVertical: 10 }}
            />
            {event.review_count && (
              <Text style={styles.reviewCount}>
                ({event.review_count} hodnocení)
              </Text>
            )}
          </View>
        )}
      </View>

      {/* Detaily akce */}
      <View style={styles.section}>
        <Text style={styles.sectionTitle}>O události</Text>
        <Text style={styles.description}>{event.description}</Text>
        
        <View style={styles.detailRow}>
          <Text style={styles.detailLabel}>Datum konání:</Text>
          <Text style={styles.detailValue}>{formatDate(event.date_of_event)}</Text>
        </View>
        
        <View style={styles.detailRow}>
          <Text style={styles.detailLabel}>Organizátor:</Text>
          <Text style={styles.detailValue}>{event.organizer}</Text>
        </View>
        
        <View style={styles.detailRow}>
          <Text style={styles.detailLabel}>Místo:</Text>
          <Text style={styles.detailValue}>{event.city}, {event.address}</Text>
        </View>
        
        <View style={styles.detailRow}>
          <Text style={styles.detailLabel}>Region:</Text>
          <Text style={styles.detailValue}>{capitalizeFirstLetter(event.region)}</Text>
        </View>
      </View>

      {/* Registrační tlačítka */}
      <View style={styles.registrationContainer}>
        <TouchableOpacity
          style={[
            styles.registrationButton,
            registrationStatus === 'visitor' && styles.registeredButton,
            registrationStatus === 'worker' && styles.disabledButton
          ]}
          onPress={() => handleRegistration('visitor')}
          disabled={isRegistering || registrationStatus === 'worker'}
        >
          <Text style={[
            styles.registrationButtonText,
            registrationStatus === 'worker' && styles.disabledButtonText
          ]}>
            {registrationStatus === 'visitor'
              ? 'Odregistrovat jako návštěvník'
              : registrationStatus === 'worker'
              ? 'Nejdříve se odregistrujte jako dobrovolník'
              : 'Registrovat jako návštěvník'}
          </Text>
        </TouchableOpacity>

        <TouchableOpacity
          style={[
            styles.registrationButton,
            registrationStatus === 'worker' && styles.registeredButton,
            registrationStatus === 'visitor' && styles.disabledButton
          ]}
          onPress={() => handleRegistration('worker')}
          disabled={isRegistering || registrationStatus === 'visitor'}
        >
          <Text style={[
            styles.registrationButtonText,
            registrationStatus === 'visitor' && styles.disabledButtonText
          ]}>
            {registrationStatus === 'worker'
              ? 'Odregistrovat jako dobrovolník'
              : registrationStatus === 'visitor'
              ? 'Nejdříve se odregistrujte jako návštěvník'
              : 'Registrovat jako dobrovolník'}
          </Text>
        </TouchableOpacity>
      </View>

      {/* Sekce recenzí */}
      <View style={styles.section}>
        <View style={styles.reviewHeader}>
          <Text style={styles.sectionTitle}>Recenze</Text>
          <TouchableOpacity 
            style={styles.addReviewButton}
            onPress={handleAddReview}
          >
            <Text style={styles.addReviewButtonText}>Přidat recenzi</Text>
          </TouchableOpacity>
        </View>

        {/* Zobrazení recenzí */}
        {event.reviews.map((review) => (
          <View key={review.id} style={styles.reviewCard}>
            <View style={styles.reviewHeader}>
              <Text style={styles.reviewAuthor}>{review.username}</Text>
              <Text style={styles.reviewDate}>
                {formatDateTime(review.date_posted)}
                {(review.edited && review.last_edited) && (
                  <Text style={styles.editedText}>
                    {`\n(upraveno ${formatDateTime(review.last_edited)})`}
                  </Text>
                )}
              </Text>
            </View>

            <View style={styles.reviewRating}>
              <Rating
                type="custom"
                ratingCount={5}
                startingValue={review.rating}
                imageSize={20}
                readonly={true}
                fractions={2}
                tintColor="#f8f8f8"
                ratingBackgroundColor="#ddd"
                style={{ paddingVertical: 5 }}
              />
            </View>

            <Text style={styles.reviewText}>{review.comment}</Text>
              
            {/* Zobrazení tlačítek pro hodnocení recenze */}
            <View style={styles.reviewActions}>
              <TouchableOpacity 
                style={[
                  styles.actionButton,
                  review.likeCount.includes(login) && styles.activeActionButton
                ]}
                onPress={() => handleLikeReview(review.id)}
              >
                <Text style={styles.actionButtonText}>
                  👍 {review.likeCount.length}
                </Text>
              </TouchableOpacity>
              
              <TouchableOpacity 
                style={[
                  styles.actionButton,
                  review.dislikeCount.includes(login) && styles.activeActionButton
                ]}
                onPress={() => handleDislikeReview(review.id)}
              >
                <Text style={styles.actionButtonText}>
                  👎 {review.dislikeCount.length}
                </Text>
              </TouchableOpacity>
              
              {/* Zobrazení tlačítka pro úpravu pro autora recenze */}
              {userProfile && userProfile.nickname === review.username && (
                <TouchableOpacity 
                  style={styles.actionButton}
                  onPress={() => handleEditReview(review.id)}
                >
                  <Text style={styles.actionButtonText}>✏️</Text>
                </TouchableOpacity>
              )}
              
              {/* Zobrazení tlačítka pro úpravu pro autora recenze a správce */}
              {(login === 'admin' || (userProfile && userProfile.nickname == review.username))&& (
                <TouchableOpacity 
                  style={styles.actionButton}
                  onPress={() => handleDeleteReview(review.id)}
                >
                  <Text style={styles.actionButtonText}>🗑️</Text>
                </TouchableOpacity>
              )}
            </View>
          </View>
        ))}
      </View>
    </ScrollView>
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
  errorText: {
    color: '#f44336',
    textAlign: 'center',
    margin: 20,
  },
  header: {
    backgroundColor: '#fff',
    padding: 20,
    borderBottomWidth: 1,
    borderBottomColor: '#e0e0e0',
  },
  title: {
    fontSize: 24,
    fontWeight: 'bold',
    marginBottom: 10,
  },
  section: {
    backgroundColor: '#fff',
    padding: 20,
    marginTop: 10,
  },
  sectionTitle: {
    fontSize: 20,
    fontWeight: '600',
    marginBottom: 15,
  },
  description: {
    fontSize: 16,
    color: '#666',
    marginBottom: 15,
  },
  detailRow: {
    flexDirection: 'row',
    marginBottom: 10,
  },
  detailLabel: {
    width: 120,
    fontSize: 16,
    fontWeight: '500',
    color: '#666',
  },
  detailValue: {
    flex: 1,
    fontSize: 16,
    color: '#333',
  },
  reviewHeader: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    marginBottom: 15,
  },
  addReviewButton: {
    backgroundColor: '#2196f3',
    paddingHorizontal: 15,
    paddingVertical: 8,
    borderRadius: 20,
  },
  addReviewButtonText: {
    color: '#fff',
    fontWeight: '500',
  },
  reviewCard: {
    backgroundColor: '#f8f8f8',
    padding: 15,
    borderRadius: 10,
    marginBottom: 10,
  },
  reviewAuthor: {
    fontSize: 16,
    fontWeight: '600',
  },
  reviewDate: {
    fontSize: 12,
    color: '#666',
  },
  reviewText: {
    fontSize: 16,
    color: '#333',
    marginVertical: 10,
  },
  rating: {
    fontSize: 16,
    marginVertical: 5,
  },
  reviewActions: {
    flexDirection: 'row',
    marginTop: 10,
    gap: 10,
  },
  actionButton: {
    paddingHorizontal: 12,
    paddingVertical: 6,
    borderRadius: 15,
    backgroundColor: '#f0f0f0',
  },
  activeActionButton: {
    backgroundColor: '#2196f3',
  },
  actionButtonText: {
    fontSize: 16,
  },
  editedText: {
    fontSize: 12,
    color: '#888',
    fontStyle: 'italic',
  },
  adminActions: {
    flexDirection: 'row',
    justifyContent: 'flex-end',
    padding: 10,
    gap: 10,
  },
  adminButton: {
    paddingHorizontal: 15,
    paddingVertical: 8,
    borderRadius: 20,
  },
  editButton: {
    backgroundColor: '#2196f3',
  },
  deleteButton: {
    backgroundColor: '#f44336',
  },
  adminButtonText: {
    color: '#fff',
    fontWeight: '500',
  },
  imageContainer: {
    width: '100%',
    height: 250,
    backgroundColor: '#f0f0f0',
    justifyContent: 'center',
    alignItems: 'center',
  },
  eventImage: {
    width: '100%',
    height: 250,
  },
  ratingContainer: {
    alignItems: 'center',
    marginTop: 5,
  },
  reviewCount: {
    fontSize: 14,
    color: '#666',
    marginTop: 5,
  },
  reviewRating: {
    alignItems: 'flex-start',
    marginVertical: 5,
  },
  registrationContainer: {
    padding: 20,
    gap: 10,
  },
  registrationButton: {
    backgroundColor: '#2196f3',
    padding: 15,
    borderRadius: 8,
    alignItems: 'center',
  },
  registeredButton: {
    backgroundColor: '#f44336',
  },
  registrationButtonText: {
    color: '#fff',
    fontSize: 16,
    fontWeight: '600',
  },
  disabledButton: {
    backgroundColor: '#e0e0e0',
  },
  disabledButtonText: {
    color: '#999',
  },
});
