import React, { useState } from 'react';
import { Button } from '@mui/material';

interface LikeDislikeButtonsProps {
  eventId: number;
  reviewId: number;
  initialLikes: string[];
  initialDislikes: string[];
  userLogin: string;
}

const LikeDislikeButtons: React.FC<LikeDislikeButtonsProps> = ({ eventId, reviewId, initialLikes, initialDislikes, userLogin}) => {
  const [likeCount, setLikeCount] = useState(initialLikes.length);
  const [dislikeCount, setDislikeCount] = useState(initialDislikes.length);
  const [liked, setLiked] = useState(initialLikes.includes(userLogin));
  const [disliked, setDisliked] = useState(initialDislikes.includes(userLogin));

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
        body: JSON.stringify({ login: userLogin }),  // Ensure this sends the correct login
      });
    }

    await fetch(`http://localhost:5000/events/${eventId}/reviews/${reviewId}/like`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ login: userLogin }),  // Ensure this sends the correct login
    });
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
        body: JSON.stringify({ login: userLogin }),  // Ensure this sends the correct login
      });
    }

    await fetch(`http://localhost:5000/events/${eventId}/reviews/${reviewId}/dislike`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ login: userLogin }),  // Ensure this sends the correct login
    });
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
