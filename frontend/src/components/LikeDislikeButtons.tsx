import React, { useState } from 'react';
import { Button } from '@mui/material';

interface LikeDislikeButtonsProps {
  eventId: number;
  reviewId: number;
  initialLikes: number;
  initialDislikes: number;
  onLike: (action: 'increment' | 'decrement') => void;
  onDislike: (action: 'increment' | 'decrement') => void;
}

const LikeDislikeButtons: React.FC<LikeDislikeButtonsProps> = ({ eventId, reviewId, initialLikes, initialDislikes, onLike, onDislike }) => {
  const [likeCount, setLikeCount] = useState(initialLikes);
  const [dislikeCount, setDislikeCount] = useState(initialDislikes);
  const [liked, setLiked] = useState(false);
  const [disliked, setDisliked] = useState(false);

  const handleLike = async () => {
    const action = liked ? 'decrement' : 'increment';
    setLiked(!liked);
    setLikeCount(likeCount + (liked ? -1 : 1));
    
    if (disliked) {
      setDisliked(false);
      setDislikeCount(dislikeCount - 1);
      await fetch(`http://localhost:5000/events/${eventId}/reviews/${reviewId}/dislike`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ action: 'decrement' }),
      });
    }

    await fetch(`http://localhost:5000/events/${eventId}/reviews/${reviewId}/like`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ action }),
    });

    onLike(action);
  };

  const handleDislike = async () => {
    const action = disliked ? 'decrement' : 'increment';
    setDisliked(!disliked);
    setDislikeCount(dislikeCount + (disliked ? -1 : 1));
    
    if (liked) {
      setLiked(false);
      setLikeCount(likeCount - 1);
      await fetch(`http://localhost:5000/events/${eventId}/reviews/${reviewId}/like`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ action: 'decrement' }),
      });
    }

    await fetch(`http://localhost:5000/events/${eventId}/reviews/${reviewId}/dislike`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ action }),
    });

    onDislike(action);
  };

  return (
    <div className="flex gap-2">
      <Button
        variant="contained"
        color={liked ? "primary" : "default"}
        onClick={handleLike}
        style={{ backgroundColor: liked ? 'green' : 'gray' }}
      >
        {likeCount} Like
      </Button>
      <Button
        variant="contained"
        color={disliked ? "secondary" : "default"}
        onClick={handleDislike}
        style={{ backgroundColor: disliked ? 'red' : 'gray' }}
      >
        {dislikeCount} Dislike
      </Button>
    </div>
  );
};

export default LikeDislikeButtons;
